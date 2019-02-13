#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include <Input.h>
#include <list>
#include <iostream>
#include "Sphere.h"
#include "Plane.h"

// This constructir has an initialisation list. It can call a base class constructor and/or be used to initialise const and normal variables.
PhysicsScene::PhysicsScene(): m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
	// Adjusted timestep from 0.01f to 0.5f for projectiles demo
}

PhysicsScene::~PhysicsScene()
{
	for (auto pAct : m_actors)
	{
		delete pAct;
	}
}

void PhysicsScene::update(float deltaTime)
{
	// This is the physics fixedUpdate function.
	// Gives the physics simulations more precision by updating it at a set interval, instead of the more varied deltaTime, and only if sufficient time has passed.
	static float accumulatedTime = 0.5f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep && !m_actors.empty())
	{
		auto itCheck = m_actors.begin();

		while (itCheck != m_actors.end())
		{
			if ((*itCheck)->getShapeID() == PLANE)
			{
				++itCheck;
				continue;
			}
			else
			{
				RigidBody* rb = dynamic_cast<RigidBody*>(*itCheck);
				assert(rb != nullptr);

				rb->fixedUpdate(m_gravity, m_timeStep);

				if (rb->getPosition().y < 0 - m_ScreenHeight / 2)
					itCheck = m_actors.erase(itCheck);
				else
					++itCheck;
			}

			// Projectiles Demo
			//updateGizmos();
		}

		if (m_actors.empty())
			break;

		accumulatedTime -= m_timeStep;
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

// Adds a physics object to the m_actors array if it doesn't already exists
bool PhysicsScene::addActor(PhysicsObject* newObj)
{
	for (auto ptr : m_actors)
	{
		if (ptr == newObj)
		{
			return false;
		}
	}

	m_actors.push_back(newObj);
	return true;
}

// Removes a physics object from the m_actors array if it exists
bool PhysicsScene::removeActor(PhysicsObject* targObj)
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		if (*it == targObj)
		{
			m_actors.erase(it);
			return true;
		}
	}

	return false;
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pAct : m_actors)
	{
		std::cout << count << ": ";
		pAct->debug();
		count++;
	}
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::planeToPlane, PhysicsScene::planeToSphere, PhysicsScene::planeToBox,
	PhysicsScene::sphereToPlane, PhysicsScene::sphereToSphere, PhysicsScene::sphereToBox,
	PhysicsScene::boxToPlane, PhysicsScene::boxToSphere, PhysicsScene::boxToBox,
};

void PhysicsScene::checkForCollision()
{
	int actorCount = (int)m_actors.size();

	for (int outer = 0; outer < actorCount - 1; outer++)
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* obj1 = m_actors[outer];
			PhysicsObject* obj2 = m_actors[inner];
			int shapeID1 = obj1->getShapeID();
			int shapeID2 = obj2->getShapeID();

			int functionIDx = (shapeID1 * SHAPE_COUNT) + shapeID2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIDx];

			if (collisionFunctionPtr != nullptr)
			{
				collisionFunctionPtr(obj1, obj2);
			}
		}
}

bool PhysicsScene::planeToPlane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::planeToSphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Plane* p = dynamic_cast<Plane*>(obj1);
	Sphere* s = dynamic_cast<Sphere*>(obj2);
	assert(p != nullptr && s != nullptr);

	glm::vec2 planeNormal = p->getNormal();
	float dis = glm::dot(planeNormal, s->getPosition()) - p->getDistance();

	if (dis < 0)
	{
		planeNormal *= -1;
		dis *= -1;
	}

	if (dis - s->getRadius() <= 0)
	{
		s->setVelocity(glm::vec2(0, 0));

		return true;
	}

	return false;
}

bool PhysicsScene::planeToBox(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::sphereToPlane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	planeToSphere(obj2, obj1);

	return false;
}

bool PhysicsScene::sphereToSphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* s1 = dynamic_cast<Sphere*>(obj1);
	Sphere* s2 = dynamic_cast<Sphere*>(obj2);

	if (s1 != nullptr && s2 != nullptr)
	{
		glm::vec2 dis = s2->getPosition() - s1->getPosition();
		float mag2 = powf(dis.x, 2) + powf(dis.y, 2);
		float radii2 = powf(s1->getRadius() + s2->getRadius(), 2);

		if (mag2 <= radii2)
		{
			s1->setVelocity(glm::vec2(0, 0));
			s2->setVelocity(glm::vec2(0, 0));

			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphereToBox(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::boxToPlane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::boxToSphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::boxToBox(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

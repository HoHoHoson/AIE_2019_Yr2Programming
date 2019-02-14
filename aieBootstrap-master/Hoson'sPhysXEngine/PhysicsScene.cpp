#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include <Input.h>
#include <list>
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

// This constructir has an initialisation list. It can call a base class constructor and/or be used to initialise const and normal variables.
PhysicsScene::PhysicsScene(): m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
	// Adjusted timestep from 0.01f to 0.5f for projectiles demo
}

PhysicsScene::~PhysicsScene()
{
	// Deletes all physics object pointers
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

	// Calls the corresponding function in the function array depending on SHAPE ID
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
		// Restitution of intersected postions after colliding
		glm::vec2 planeIntersect = s->getPosition() - (planeNormal * dis);
		s->setPosition(planeIntersect + planeNormal * s->getRadius());

		p->resolveCollision(planeNormal, s);

		return true;
	}

	return false;
}

bool PhysicsScene::planeToBox(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Plane* p = dynamic_cast<Plane*>(obj1);
	Box* b = dynamic_cast<Box*>(obj2);
	assert(p != nullptr && b != nullptr);

	glm::vec2 pNormal = p->getNormal();
	float pDistance = p->getDistance();

	glm::vec2 bPos = b->getPosition();
	glm::vec2 bExtents = b->getExtents();

	if ((glm::dot(pNormal, bPos) - pDistance) < 0)
	{
		pDistance *= -1;
		pNormal *= -1;
	}

	float b1 = glm::dot(pNormal, bPos + bExtents) - pDistance;
	float b2 = glm::dot(pNormal, bPos - bExtents) - pDistance;
	float b3 = glm::dot(pNormal, glm::vec2(bPos.x + bExtents.x, bPos.y - bExtents.y)) - pDistance;
	float b4 = glm::dot(pNormal, glm::vec2(bPos.x - bExtents.x, bPos.y + bExtents.y)) - pDistance;

	if (b1 > 0 && b2 > 0 && b3 > 0 && b4 > 0)
	{
		return false;
	}
	else
	{
		b->setVelocity(glm::vec2(0, 0));
		return true;
	}
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
			// Restitution of intersected postions after colliding
			float intersectLength = (sqrtf(radii2) - sqrtf(mag2));
			glm::vec2 collisionNormal = glm::normalize(s2->getPosition() - s1->getPosition());
			float totalMass = s1->getMass() + s2->getMass();

			s1->setPosition(s1->getPosition() - ((s1->getMass() / totalMass) * intersectLength) * collisionNormal);
			s2->setPosition(s2->getPosition() + ((s2->getMass() / totalMass) * intersectLength) * collisionNormal);

			s1->resolveCollision(s2);

			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphereToBox(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere* s = dynamic_cast<Sphere*>(obj1);
	Box* b = dynamic_cast<Box*>(obj2);
	assert(s != nullptr && b != nullptr);

	float xMax = b->getPosition().x + b->getExtents().x;
	float xMin = b->getPosition().x - b->getExtents().x;
	float yMax = b->getPosition().y + b->getExtents().y;
	float yMin = b->getPosition().y - b->getExtents().y;

	glm::vec2 closestPoint(s->getPosition());

	if (closestPoint.x > xMax)
		closestPoint.x = xMax;
	else if (closestPoint.x < xMin)
		closestPoint.x = xMin;

	if (closestPoint.y > yMax)
		closestPoint.y = yMax;
	else if (closestPoint.y < yMin)
		closestPoint.y = yMin;

	closestPoint -= s->getPosition();
	float mag2 = powf(closestPoint.x, 2) + powf(closestPoint.y, 2);

	if (mag2 <= powf(s->getRadius(), 2))
	{
		s->setVelocity(glm::vec2(0, 0));
		b->setVelocity(glm::vec2(0, 0));

		return true;
	}
	else
		return false;
}

bool PhysicsScene::boxToPlane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	planeToBox(obj2, obj1);

	return false;
}

bool PhysicsScene::boxToSphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	sphereToBox(obj2, obj1);

	return false;
}

bool PhysicsScene::boxToBox(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Box* b1 = dynamic_cast<Box*>(obj1);
	Box* b2 = dynamic_cast<Box*>(obj2);
	assert(b1 != nullptr && b2 != nullptr);

	float b1_xMax = b1->getPosition().x + b1->getExtents().x;
	float b1_xMin = b1->getPosition().x - b1->getExtents().x;
	float b1_yMax = b1->getPosition().y + b1->getExtents().y;
	float b1_yMin = b1->getPosition().y - b1->getExtents().y;
	float b2_xMax = b2->getPosition().x + b2->getExtents().x;
	float b2_xMin = b2->getPosition().x - b2->getExtents().x;
	float b2_yMax = b2->getPosition().y + b2->getExtents().y;
	float b2_yMin = b2->getPosition().y - b2->getExtents().y;

	if (b1_xMax < b2_xMin || b2_xMax < b1_xMin ||
		b1_yMax < b2_yMin || b2_yMax < b1_yMin)
	{
		return false;
	}
	else
	{
		b1->setVelocity(glm::vec2(0, 0));
		b2->setVelocity(glm::vec2(0, 0));

		return true;
	}
}

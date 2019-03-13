#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include <Input.h>
#include <list>
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "Shape.h"
#include "Box.h"

// This constructir has an initialisation list. It can call a base class constructor and/or be used to initialise const and normal variables.
PhysicsScene::PhysicsScene(): m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
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
		}

		if (m_actors.empty())
			break;

		accumulatedTime -= m_timeStep;
	}
}

void PhysicsScene::updateGizmos() const
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

typedef bool(*fPtr)(PhysicsObject*, PhysicsObject*);

static fPtr collisionSATArray[] =
{
	PhysicsScene::planeToPlane, PhysicsScene::planeToShape,
	PhysicsScene::shapeToPlane, PhysicsScene::shapeToShape
};

void PhysicsScene::checkForCollision() const
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

			// Temporary until the SAT collision system is implemented
			if (shapeID1 == SHAPE || shapeID2 == SHAPE)
				continue;

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

bool PhysicsScene::planeToShape(PhysicsObject* obj1, PhysicsObject* pbj2)
{
	return false;
}

bool PhysicsScene::shapeToPlane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::shapeToShape(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::planeToSphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Plane* p = dynamic_cast<Plane*>(obj1);
	Sphere* s = dynamic_cast<Sphere*>(obj2);
	assert(p != nullptr && s != nullptr);

	float dis = glm::dot(p->getNormal(), s->getPosition()) - p->getDistance();

	if (dis - s->getRadius() < 0)
	{
		// Restitution of intersected postions after colliding
		glm::vec2 planeIntersect = s->getPosition() - (p->getNormal() * dis);
		s->setPosition(planeIntersect + p->getNormal() * s->getRadius());

		p->resolveCollision(s);

		return true;
	}

	return false;
}

bool PhysicsScene::planeToBox(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Plane* p = dynamic_cast<Plane*>(obj1);
	Box* b = dynamic_cast<Box*>(obj2);
	assert(p && b);

	float distanceMax = 0;

	for (float x = b->getPosition().x - b->getExtents().x; x <= (b->getPosition().x + b->getExtents().x); x += (b->getExtents().x * 2))
	{
		for (float y = b->getPosition().y - b->getExtents().y; y <= (b->getPosition().y + b->getExtents().y); y += (b->getExtents().y * 2))
		{
			glm::vec2 point(x, y);
			float distance = glm::dot(point, p->getNormal()) - p->getDistance();

			if (distance < distanceMax)
				distanceMax = distance;
		}
	}

	if (distanceMax < 0)
	{
		b->setPosition(b->getPosition() + (p->getNormal() * distanceMax * -1.0f));
		p->resolveCollision(b);

		return true;
	}

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
	assert(s1 != nullptr && s2 != nullptr);

	glm::vec2 dis = s2->getPosition() - s1->getPosition();
	float mag2 = powf(dis.x, 2) + powf(dis.y, 2);
	float radii2 = powf(s1->getRadius() + s2->getRadius(), 2);

	if (mag2 < radii2)
	{
		// Restitution of intersected postions after colliding
		float intersectLength = (sqrtf(radii2) - sqrtf(mag2));
		glm::vec2 collisionNormal = glm::normalize(s2->getPosition() - s1->getPosition());
		float totalMass = s1->getMass() + s2->getMass();

		s1->setPosition(s1->getPosition() - (((1 - (s1->getMass()) / totalMass)) * intersectLength) * collisionNormal);
		s2->setPosition(s2->getPosition() + (((1 - (s2->getMass()) / totalMass)) * intersectLength) * collisionNormal);

		glm::vec2 contact = 0.5f * (s1->getPosition() + s2->getPosition());

		s1->resolveCollision(s2);

		return true;
	}

	return false;
}

bool PhysicsScene::sphereToBox(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere* s = dynamic_cast<Sphere*>(obj1);
	Box* b = dynamic_cast<Box*>(obj2);
	assert(s && b);

	glm::vec2 contactPoint = s->getPosition();

	float xMin = b->getPosition().x - b->getExtents().x;
	float xMax = b->getPosition().x + b->getExtents().x;
	float yMin = b->getPosition().y - b->getExtents().y;
	float yMax = b->getPosition().y + b->getExtents().y;

	if (contactPoint.x < xMin)
		contactPoint.x = xMin;
	else if (contactPoint.x > xMax)
		contactPoint.x = xMax;

	if (contactPoint.y < yMin)
		contactPoint.y = yMin;
	else if (contactPoint.y > yMax)
		contactPoint.y = yMax;

	glm::vec2 displacement = contactPoint - s->getPosition();
	float magPow2 = (displacement.x * displacement.x) + (displacement.y * displacement.y);

	if ((magPow2 - (s->getRadius() * s->getRadius())) < 0)
	{
		s->resolveCollision(b);

		return true;
	}

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
	assert(b1 && b2);

	float xMinb1 = b1->getPosition().x - b1->getExtents().x;
	float xMaxb1 = b1->getPosition().x + b1->getExtents().x;
	float yMinb1 = b1->getPosition().y - b1->getExtents().y;
	float yMaxb1 = b1->getPosition().y + b1->getExtents().y;

	float xMinb2 = b2->getPosition().x - b2->getExtents().x;
	float xMaxb2 = b2->getPosition().x + b2->getExtents().x;
	float yMinb2 = b2->getPosition().y - b2->getExtents().y;
	float yMaxb2 = b2->getPosition().y + b2->getExtents().y;

	if (xMinb1 > xMaxb2 || xMinb2 > xMaxb1 || yMinb1 > yMaxb2 || yMinb2 > yMaxb1)
		return false;
	
	b1->resolveCollision(b2);

	return true;
}

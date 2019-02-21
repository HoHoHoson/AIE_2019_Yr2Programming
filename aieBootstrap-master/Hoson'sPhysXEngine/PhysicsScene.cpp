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

	if (dis - s->getRadius() < 0)
	{
		// Restitution of intersected postions after colliding
		glm::vec2 planeIntersect = s->getPosition() - (planeNormal * dis);
		s->setPosition(planeIntersect + planeNormal * s->getRadius());

		float KEpreCollision = s->getKineticEnergy();

		p->resolveCollision(planeNormal, s, planeIntersect);

		float KEpostCollision = s->getKineticEnergy();
		float KEdelta = KEpostCollision - KEpreCollision;

		if (KEdelta < -0.01f || KEdelta > 0.01f)
		{
			std::cout << "Plane-Sphere Energy Change Detected" << std::endl;
		}

		return true;
	}

	return false;
}

bool PhysicsScene::planeToBox(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Plane* p = dynamic_cast<Plane*>(obj1);
	Box* b = dynamic_cast<Box*>(obj2);
	assert(p != nullptr && b != nullptr);

	int numContacts = 0;
	glm::vec2 contact(0, 0);
	float contactV = 0;
	float radius = std::fminf(b->getExtents().x, b->getExtents().y);
	float penetration = 0;

	glm::vec2 planeOrigin = p->getNormal() * p->getDistance();
	float comFromPlane = glm::dot(b->getPosition() - planeOrigin, p->getNormal());

	for (float x = -b->getExtents().x; x < (b->getExtents().x * 2); x += (b->getExtents().x * 2))
	{
		for (float y = -b->getExtents().y; y < (b->getExtents().y * 2); y += (b->getExtents().y * 2))
		{
			glm::vec2 bCorner = b->getPosition() + x * b->getLocalX() + y * b->getLocalY();

			float distFromPlane = glm::dot(bCorner - planeOrigin, p->getNormal());

			float velocityIntoPlane = glm::dot(b->getVelocity() + b->getAngularVelocity() * (-y * b->getLocalX() + x * b->getLocalY()), p->getNormal());

			if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane >= 0) ||
				(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane <= 0))
			{
				numContacts++;
				contact += bCorner;
				contactV += velocityIntoPlane;

				if (comFromPlane >= 0)
				{
					if (penetration > distFromPlane)
					{
						penetration = distFromPlane;
					}
				}
				else if (penetration < distFromPlane)
				{
					penetration = distFromPlane;
				}
			}
		}
	}

	if (numContacts > 0)
	{
		float collisionV = contactV / (float)numContacts;

		glm::vec2 acceleration = -p->getNormal() * ((1.0f + b->getElasticity()) * collisionV);

		glm::vec2 localContact = (contact / (float)numContacts) - b->getPosition();
		float r = glm::dot(localContact, glm::vec2(p->getNormal().y, -p->getNormal().x));

		float mass0 = 1.0f / (1.0f / b->getMass() + (r * r) / b->getMoment());

		float KEpreCollision = b->getKineticEnergy();

		b->applyForce(acceleration * mass0, localContact);

		float KEpostCollision = b->getKineticEnergy();
		float KEdelta = KEpostCollision - KEpreCollision;
		if (KEdelta < -0.01f || KEdelta > 0.01f)
		{
			std::cout << "Plane-Box Energy Change Detected" << std::endl;
		}

		b->setPosition(b->getPosition() - p->getNormal() * penetration);

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
		
		float KEpreCollision = s1->getKineticEnergy() + s2->getKineticEnergy();

		s1->resolveCollision(s2, contact);

		float KEpostCollision = s1->getKineticEnergy() + s2->getKineticEnergy();
		float KEdelta = KEpostCollision - KEpreCollision;

		if (KEdelta < -0.01f || KEdelta > 0.01f)
		{
			std::cout << "Sphere-Sphere Energy Change Detected" << std::endl;
		}

		return true;
	}

	return false;
}

bool PhysicsScene::sphereToBox(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere* s = dynamic_cast<Sphere*>(obj1);
	Box* b = dynamic_cast<Box*>(obj2);
	assert(s != nullptr && b != nullptr);

	glm::vec2 displacement = s->getPosition() - b->getPosition();
	float w2 = b->getExtents().x;
	float h2 = b->getExtents().y;

	int numContacts = 0;
	glm::vec2 contact(0, 0);

	for (float x = -w2; x <= w2; x += (b->getExtents().x * 2))
	{
		for (float y = -h2; y <= h2; y += (b->getExtents().y * 2))
		{
			glm::vec2 bCorner = x * b->getLocalX() + y * b->getLocalY();
			glm::vec2 dp = bCorner - displacement;

			if (dp.x * dp.x + dp.y * dp.y < s->getRadius() * s->getRadius())
			{
				numContacts++;
				contact += glm::vec2(x, y);
			}
		}
	}

	glm::vec2* direction = nullptr;
	glm::vec2 localPos(glm::dot(b->getLocalX(), displacement), glm::dot(b->getLocalY(), displacement));

	if (localPos.y < h2 && localPos.y > -h2)
	{
		if (localPos.x > 0 && localPos.x < w2 + s->getRadius())
		{
			numContacts++;
			contact += glm::vec2(w2, localPos.y);
			direction = new glm::vec2(b->getLocalX());
		}

		if (localPos.x < 0 && localPos.x > -(w2 + s->getRadius()))
		{
			numContacts++;
			contact += glm::vec2(-w2, localPos.y);
			direction = new glm::vec2(-b->getLocalX());
		}
	}

	if (localPos.x < w2 && localPos.x > -w2)
	{
		if (localPos.y > 0 && localPos.y < h2 + s->getRadius())
		{
			numContacts++;
			contact += glm::vec2(localPos.x, h2);
			direction = new glm::vec2(b->getLocalY());
		}

		if (localPos.y < 0 && localPos.y > -(h2 + s->getRadius()))
		{
			numContacts++;
			contact += glm::vec2(localPos.x, -h2);
			direction = new glm::vec2(-b->getLocalY());
		}
	}

	if (numContacts > 0)
	{
		contact = b->getPosition() + (1.0f / numContacts) * (b->getLocalX() * contact.x + b->getLocalY() * contact.y);

		float KEpreCollision = b->getKineticEnergy() + s->getKineticEnergy();

		s->resolveCollision(b, contact, direction);

		float KEpostCollision = b->getKineticEnergy() + s->getKineticEnergy();
		float KEdelta = KEpostCollision - KEpreCollision;
		if (KEdelta < -0.01f || KEdelta > 0.01f)
		{
			std::cout << "Sphere-Box Energy Change Detected" << std::endl;
		}

		delete direction;
		return true;
	}

	delete direction;
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

	glm::vec2 boxPos = b2->getPosition() - b1->getPosition();
	glm::vec2 normal(0, 0);
	glm::vec2 contactForce1, contactForce2;
	glm::vec2 contact(0, 0);
	int numContacts = 0;

	b1->checkBoxCorners(*b2, contact, numContacts, normal, contactForce1);

	if (b2->checkBoxCorners(*b1, contact, numContacts, normal, contactForce2)) 
	{
		normal= -normal;
	}

	if (numContacts > 0)
	{
		glm::vec2 contactForce = 0.5f * (contactForce1 - contactForce2);
		b1->setPosition(b1->getPosition() - contactForce);
		b2->setPosition(b2->getPosition() + contactForce);

		float KEpreCollision = b1->getKineticEnergy() + b2->getKineticEnergy();

		b1->resolveCollision(b2, contact / float(numContacts), &normal);

		float KEpostCollision = b1->getKineticEnergy() + b2->getKineticEnergy();
		float KEdelta = KEpostCollision - KEpreCollision;
		if (KEdelta < -0.01f || KEdelta > 0.01f)
		{
			std::cout << "Box-Box Energy Change Detected" << std::endl;
		}

		return true;
	}

	return false;
}

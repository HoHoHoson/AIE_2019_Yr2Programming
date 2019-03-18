#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include <Input.h>
#include <list>
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "Shape.h"
#include <Gizmos.h>
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
		vec2 h = vec2(0);
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

			int functionIDx = (shapeID1 * SHAPE_COUNT) + shapeID2;
			fPtr SATfunctionPtr = collisionSATArray[functionIDx];

			if (SATfunctionPtr != nullptr)
			{
				SATfunctionPtr(obj1, obj2);
			}
		}
}

bool PhysicsScene::planeToPlane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::planeToShape(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane* p = dynamic_cast<Plane*>(obj1);
	Shape* s = dynamic_cast<Shape*>(obj2);
	assert(p && s);

	if (s->getVertices() == 0)
	{
		float dis = glm::dot(p->getNormal(), s->getPosition()) - p->getDistance();

		if (dis - s->getRadius() < 0)
		{
			// Restitution of intersected postions after colliding
			glm::vec2 planeIntersect = s->getPosition() - (p->getNormal() * dis);
			s->setPosition(planeIntersect + p->getNormal() * s->getRadius());

			s->resolveCollision(p);

			return true;
		}

		return false;
	}
	else
	{
		float* minDistance = nullptr;
		vec2 minPoint;

		for (int i = 0; i < s->getVertices(); ++i)
		{
			vec2 point = s->getVertice(i);
			float dis = glm::dot(point, p->getNormal()) - p->getDistance();

			if (minDistance == nullptr)
			{
				minDistance = new float(dis);
				minPoint = point;
			}
			else if (dis < *minDistance)
			{
				*minDistance = dis;
				minPoint = point;
			}
		}

		if (*minDistance < 0)
		{
			vec2 restitution = p->getNormal() * -*minDistance;
			s->setPosition(s->getPosition() + restitution);

			delete minDistance;

			s->resolveCollision(p);
			return true;
		}

		delete minDistance;

		return false;
	}

	return false;
}

bool PhysicsScene::shapeToPlane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	planeToShape(obj2, obj1);
	return false;
}

bool PhysicsScene::shapeToShape(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Shape* s1 = dynamic_cast<Shape*>(obj1);
	Shape* s2 = dynamic_cast<Shape*>(obj2);
	assert(s1 && s2);

	if (s1->getVertices() == 0 && s2->getVertices() == 0)
	{
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

			s1->resolveCollision(s2, &collisionNormal);

			return true;
		}

		return false;
	}
	else if (s1->getVertices() == 0 || s2->getVertices() == 0)
	{
		Shape* circle = nullptr;
		Shape* polygon = nullptr;

		if (s1->getVertices() == 0)
		{
			circle = s1;
			polygon = s2;
		}
		else
		{
			circle = s2;
			polygon = s1;
		}

		vec2 pointA;
		vec2 pointB;
		float* minDistance = nullptr;

		for (int i = 0; i < polygon->getVertices(); ++i)
		{
			vec2 p1 = polygon->getVertice(i);
			vec2 p2 = polygon->getVertice(i + 1);
			vec2 p1C = circle->getPosition() - p1;
			vec2 p2C = circle->getPosition() - p2;

			float totalDisSqr = (p1C.x * p1C.x) + (p1C.y * p1C.y) + (p2C.x * p2C.x) + (p2C.y * p2C.y);
			
			if (minDistance == nullptr)
			{
				minDistance = new float(totalDisSqr);
				pointA = p1;
				pointB = p2;
			}
			else if (totalDisSqr < *minDistance)
			{
				*minDistance = totalDisSqr;
				pointA = p1;
				pointB = p2;
			}
		}

		vec2 closestEdge = pointB - pointA;
		vec2 eDisplacement = circle->getPosition() - pointA;
		float edgeLengthSqr = (closestEdge.x * closestEdge.x) + (closestEdge.y * closestEdge.y);
		float t = glm::dot(eDisplacement, closestEdge) / edgeLengthSqr;

		if (t < 0)
			t = 0;
		else if (t > 1)
			t = 1;

		vec2 closestPoint = pointA + t * closestEdge;
		vec2 pDisplacement = circle->getPosition() - closestPoint;
		float disSqr = (pDisplacement.x * pDisplacement.x) + (pDisplacement.y * pDisplacement.y);
		float radiusSqr = powf(circle->getRadius(), 2);

		if (disSqr < radiusSqr)
		{
			float pen = (1 - disSqr / radiusSqr) * circle->getRadius();
			float totalMass = circle->getMass() + polygon->getMass();
			vec2 collisionNorm = glm::normalize(pDisplacement);
			vec2 cRestitution = (1 - circle->getMass() / totalMass) * collisionNorm * pen;
			vec2 pRestitution = (1 - polygon->getMass() / totalMass) * collisionNorm * pen;

			polygon->setPosition(polygon->getPosition() - pRestitution);
			circle->setPosition(circle->getPosition() + cRestitution);
			closestPoint -= pRestitution;

			circle->resolveCollision(polygon, &collisionNorm);
			return true;
		}

		return false;
	}
	else
	{
		Shape* pointsShape = nullptr;
		vec2 closestPoint;
		vec2 axis;
		float penetration;

		if (isSATintersect(s1, s2, axis, penetration, closestPoint, pointsShape))
		{
			if (isSATintersect(s2, s1, axis, penetration, closestPoint, pointsShape))
			{
				// Makes sure the axis is pointing in the correct direction, away from its origin shape
				if (pointsShape == s1)
				{
					if (glm::dot(closestPoint - s2->getPosition(), axis) < 0)
						axis = -axis;
				}
				else if (glm::dot(closestPoint - s1->getPosition(), axis) < 0)
					axis = -axis;

				// Collision Restitution
				float totalMass = s1->getMass() + s2->getMass();
				vec2 s1Restitution = (1 - s1->getMass() / totalMass) * penetration * axis;
				vec2 s2Restitution = (1 - s2->getMass() / totalMass) * penetration * axis;

				if (pointsShape == s1)
				{
					s1->setPosition(s1->getPosition() + s1Restitution);
					s2->setPosition(s2->getPosition() - s2Restitution);

					closestPoint += s1Restitution;
					s2->resolveCollision(s1, &axis);
				}
				else
				{
					s1->setPosition(s1->getPosition() - s1Restitution);
					s2->setPosition(s2->getPosition() + s2Restitution);

					closestPoint += s2Restitution;
					s1->resolveCollision(s2, &axis);
				}

				return true;
			}
		}
		else
			return false;
	}

	return false;
}

void PhysicsScene::setSATmaxmin(const vec2& axis, Shape* s, float& min, float& max, vec2*& pMin, vec2*& pMax)
{
	for (int j = 0; j < s->getVertices(); ++j)
	{
		float dot = glm::dot(axis, s->getVertice(j));

		if (!pMin && !pMax)
		{
			pMin = new vec2(s->getVertice(j));
			pMax = new vec2(s->getVertice(j));
			min = dot;
			max = dot;

			continue;
		}

		if (dot < min)
		{
			min = dot;
			*pMin = s->getVertice(j);
		}
		else if (dot > max)
		{
			max = dot;
			*pMax = s->getVertice(j);
		}
	}
}

bool PhysicsScene::isSATintersect(Shape* mainS, Shape* secondaryS, vec2& axis, float& pen, vec2& closestPoint, Shape*& pointsShape)
{
	int mainVertices = mainS->getVertices() % 2 == 0 ? mainS->getVertices() / 2 : mainS->getVertices();

	for (int i = 0; i < mainVertices; ++i)
	{
		vec2 start(mainS->getVertice(i));
		vec2 end(mainS->getVertice(i + 1));

		vec2 perp = glm::normalize(end - start);
		perp = glm::vec2(-perp.y, perp.x);

		float s1Min;
		float s1Max;
		vec2* s1pMin = nullptr;
		vec2* s1pMax = nullptr;

		float s2Min;
		float s2Max;
		vec2* s2pMin = nullptr;
		vec2* s2pMax = nullptr;

		setSATmaxmin(perp, mainS, s1Min, s1Max, s1pMin, s1pMax);
		setSATmaxmin(perp, secondaryS, s2Min, s2Max, s2pMin, s2pMax);

		if (s1Min > s2Max || s2Min > s1Max)
		{
			delete s1pMin;
			delete s1pMax;
			delete s2pMin;
			delete s2pMax;

			return false;
		}
		else
		{
			float penetration;
			vec2 cPoint;

			if ((s1Max - s2Min) < (s2Max - s1Min))
			{
				penetration = s1Max - s2Min;
				cPoint = *s2pMin;
			}
			else
			{
				penetration = s2Max - s1Min;
				cPoint = *s2pMax;
			}

			if (!pointsShape)
			{
				pen = penetration;
				pointsShape = secondaryS;
				closestPoint = cPoint;
				axis = perp;
			}
			else if (penetration < pen)
			{
				pointsShape = secondaryS;
				closestPoint = cPoint;
				pen = penetration;
				axis = perp;
			}

			delete s1pMin;
			delete s1pMax;
			delete s2pMin;
			delete s2pMax;
		}
	}

	return true;
}

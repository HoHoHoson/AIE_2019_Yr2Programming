#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include <Input.h>
#include <list>
#include <iostream>
#include "Sphere.h"

// This constructir has an initialisation list. It can call a base class constructor and/or be used to initialise const and normal variables.
PhysicsScene::PhysicsScene(): m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
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
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep && !m_actors.empty())
	{
		auto itCheck = m_actors.begin();
		Sphere* sphereCheck;

		while (itCheck != m_actors.end())
		{
			sphereCheck = dynamic_cast<Sphere*>(*itCheck);
			sphereCheck->fixedUpdate(m_gravity, m_timeStep);

			if (sphereCheck->getPosition().y < 0 - m_ScreenHeight / 2)
				itCheck = m_actors.erase(itCheck);
			else
				++itCheck;
		}

		if (m_actors.empty())
			break;

		// Sphere collision detection
		//auto iterator = m_actors.begin();
		//itCheck = iterator++;

		//sphereCheck = dynamic_cast<Sphere*>(*itCheck);
		//Sphere* sphereOther;

		//while (itCheck != m_actors.end())
		//{
		//	if (sphereCheck->checkCollision(*iterator))
		//	{
		//		sphereOther = dynamic_cast<Sphere*>(*iterator);
		//		sphereCheck->applyForceToActor(sphereOther, sphereCheck->getMass() * sphereCheck->getVelocity());
		//	}

		//	++iterator;

		//	if (iterator == m_actors.end())
		//	{
		//		iterator = ++itCheck;
		//		++iterator;

		//		if (iterator == m_actors.end())
		//			++itCheck;
		//		else
		//			sphereCheck = dynamic_cast<Sphere*>(*itCheck);
		//	}
		//}

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
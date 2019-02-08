#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include <Input.h>
#include <list>
#include <iostream>
#include "Sphere.h"
#include "Hoson_sPhysXEngineApp.h"

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
	static std::list<PhysicsObject*> dirty;

	// This is the physics fixedUpdate function.
	// Gives the physics simulations more precision by updating it at a set interval, instead of the more varied deltaTime, and only if sufficient time has passed.
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActors : m_actors)
		{	
			RigidBody* pObj = dynamic_cast<RigidBody*>(pActors);

			if (pObj->getPosition().y < )

			pActors->fixedUpdate(m_gravity, m_timeStep);
		}

		//for (auto pAct : m_actors)
		//{
		//	for (auto pOther : m_actors)
		//	{
		//		if (pAct == pOther)
		//		{
		//			continue;
		//		}

		//		if (std::find(dirty.begin(), dirty.end(), pAct) != dirty.end() && std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
		//		{
		//			continue;
		//		}

		//		RigidBody* pRigid = dynamic_cast<RigidBody*>(pAct);
		//		if (pRigid->checkCollision(pOther))
		//		{
		//			pRigid->applyForceToActor(dynamic_cast<RigidBody*>(pOther), pRigid->getVelocity() * pRigid->getMass());
		//			dirty.push_back(pAct);
		//			dirty.push_back(pOther);
		//		}
		//	}
		//}

		//dirty.clear();

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
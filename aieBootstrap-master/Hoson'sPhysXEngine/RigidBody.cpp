#include <iostream>
#include "RigidBody.h"
#include "Plane.h"

#define MIN_LINEAR_THRESEHOLD 0.01
#define MIN_ROTATION_THRESEHOLD 0.01

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass, float drag, float elasticity) : PhysicsObject(shapeID)
{
	m_Position = position;
	m_Velocity = velocity;
	m_LinearDrag = drag;

	m_Mass = mass;
	m_Elasticity = elasticity;
}

RigidBody::~RigidBody()
{

}

void RigidBody::applyForce(glm::vec2 force)
{
	m_Velocity += force / m_Mass;
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_Position += m_Velocity * timeStep;
	m_Velocity += gravity * timeStep;
	m_Velocity -= m_Velocity * m_LinearDrag * timeStep;

	if ((powf(m_Velocity.x, 2) + powf(m_Velocity.y, 2)) < powf(MIN_LINEAR_THRESEHOLD, 2))
	{
		m_Velocity = glm::vec2(0, 0);
	}
}

void RigidBody::resolveCollision(RigidBody* other, glm::vec2* collisionNormal)
{
	// Collision Normal
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : other->getPosition() - m_Position);
	glm::vec2 relativeVel = other->getVelocity() - m_Velocity;

	float elasticity = (m_Elasticity + other->getElasticity()) / 2.0f;
	float j = glm::dot(-(1 + elasticity) * (relativeVel), normal) / glm::dot(normal, normal * ((1 / m_Mass) + (1 / other->getMass())));
	glm::vec2 force = j * normal;

	applyForce(-force);
	other->applyForce(force);
}

void RigidBody::resolveCollision(Plane * plane)
{
	float j = glm::dot(-(1.0f + m_Elasticity) * m_Velocity, plane->getNormal()) / (1 / m_Mass);
	glm::vec2 force = j * plane->getNormal();

	applyForce(force);
}

#include "RigidBody.h"

#define MIN_LINEAR_THRESEHOLD 0.1
#define MIN_ROTATION_THRESEHOLD 0.01

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float drag, float angularDrag, float elasticity) : PhysicsObject(shapeID)
{
	m_Position = position;
	m_Velocity = velocity;
	m_Mass = mass;
	m_Rotation = rotation;
	m_LinearDrag = drag;
	m_AngularDrag = angularDrag;
	m_Elasticity = elasticity;
}

RigidBody::~RigidBody()
{

}

void RigidBody::applyForce(glm::vec2 force)
{
	m_Velocity += force / m_Mass;
}

void RigidBody::applyForceToActor(RigidBody* pOther, glm::vec2 force)
{
	pOther->applyForce(-force);
	applyForce(force);
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if ((powf(m_Velocity.x, 2) + powf(m_Velocity.y, 2)) < powf(MIN_LINEAR_THRESEHOLD, 2))
	{
		m_Velocity = glm::vec2(0, 0);
	}
	else
	{
		m_Velocity -= m_Velocity * m_LinearDrag * timeStep;
	}
	// Implement in the future: m_AngularVelocity -= m_AngularVelocity * m_AngularDrag * timeStep;

	applyForce(gravity * m_Mass * timeStep);

	m_Position += m_Velocity * timeStep;
}

void RigidBody::resolveCollision(RigidBody* other)
{
	// Collision Normal
	glm::vec2 normal = glm::normalize(other->getPosition() - m_Position);
	glm::vec2 relativeVel = other->getVelocity() - m_Velocity;

	float elasticity = (m_Elasticity + other->getElasticity()) / 2.0f;
	float j = glm::dot(-(1 + elasticity) * (relativeVel), normal) / glm::dot(normal, normal * ((1 / m_Mass) + (1 / other->getMass())));

	glm::vec2 force = normal * j;

	// Applies force to both itself and the collided object
	applyForceToActor(other, -force);
}
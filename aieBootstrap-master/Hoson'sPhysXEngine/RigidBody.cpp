#include "RigidBody.h"

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) : PhysicsObject(shapeID)
{
	m_Position = position;
	m_Velocity = velocity;
	m_Mass = mass;
	m_Rotation = rotation;
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
	applyForce(gravity * m_Mass * timeStep);
	m_Position += m_Velocity * timeStep;
}
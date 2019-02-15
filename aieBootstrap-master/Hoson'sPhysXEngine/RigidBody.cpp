#include "RigidBody.h"

#define MIN_LINEAR_THRESEHOLD 0.1
#define MIN_ROTATION_THRESEHOLD 0.01

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float drag, float angularDrag, float elasticity) : PhysicsObject(shapeID)
{
	m_Position = position;
	m_Velocity = velocity;
	m_LinearDrag = drag;

	m_Mass = mass;
	m_Elasticity = elasticity;

	m_Rotation = rotation;
	m_AngularVelocity = 0;
	m_AngularDrag = angularDrag;
}

RigidBody::~RigidBody()
{

}

void RigidBody::applyForce(glm::vec2 force, glm::vec2 contactPos)
{
	m_Velocity += force / m_Mass;
	m_AngularVelocity += (force.y * contactPos.x - force.x * contactPos.y) / m_Moment;
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_Position += m_Velocity * timeStep;
	m_Velocity += gravity * timeStep;
	m_Velocity -= m_Velocity * m_LinearDrag * timeStep;

	m_Rotation += m_AngularVelocity * timeStep;
	m_AngularVelocity -= m_AngularVelocity * m_AngularDrag * timeStep;

	if ((powf(m_Velocity.x, 2) + powf(m_Velocity.y, 2)) < powf(MIN_LINEAR_THRESEHOLD, 2))
	{
		m_Velocity = glm::vec2(0, 0);
	}

	float angVel = m_AngularVelocity;
	if (angVel < 0)
		angVel *= -1;

	if (angVel < MIN_ROTATION_THRESEHOLD)
		m_AngularVelocity = 0;
}

void RigidBody::resolveCollision(RigidBody* other, glm::vec2 contact, glm::vec2* collsionNormal)
{
	// Collision Normal
	glm::vec2 normal = glm::normalize(other->getPosition() - m_Position);
	glm::vec2 relativeVel = other->getVelocity() - m_Velocity;

	glm::vec2 perp(normal.y, -normal.x);

	float r1 = glm::dot(contact - m_Position, -perp);
	float r2 = glm::dot(contact - other->m_Position, perp);
	float v1 = glm::dot(m_Velocity, normal) - r1 * m_AngularVelocity;
	float v2 = glm::dot(other->m_Velocity, normal) + r2 * other->m_AngularVelocity;

	if (v1 > v2)
	{
		float mass1 = 1.0f / (1.0f / m_Mass + (r1 * r1) / m_Moment);
		float mass2 = 1.0f / (1.0f / other->m_Mass + (r2 * r2) / other->m_Moment);

		float elasticity = (m_Elasticity + other->getElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		applyForce(-force, contact - m_Position);
		other->applyForce(force, contact - other->m_Position);
	}
}
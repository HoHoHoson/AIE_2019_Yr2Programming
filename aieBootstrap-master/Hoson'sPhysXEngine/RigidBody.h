#pragma once
#include "PhysicsObject.h"

class Plane;

class RigidBody : public PhysicsObject 
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass, float drag, float elasticity);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {};
	void applyForce(glm::vec2 force);

	glm::vec2 getPosition()		const { return m_Position; }
	glm::vec2 getVelocity()		const { return m_Velocity; }
	float getMass()				const { return m_Mass; }
	float getLinearDrag()		const { return m_LinearDrag; }
	float getElasticity()		const { return m_Elasticity; }

	void setPosition(glm::vec2 newPos) { m_Position = newPos; }
	void setVelocity(glm::vec2 newVel) { m_Velocity = newVel; }
	void setMass(float newMass) { m_Mass = newMass; }
	float setLinearDrag(float newValue) { m_LinearDrag = newValue; }

	void resolveCollision(RigidBody* other, glm::vec2* collisionNormal = nullptr);
	void resolveCollision(Plane* plane);

protected:

	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	float m_LinearDrag;

	float m_Mass;
	float m_Elasticity;
};
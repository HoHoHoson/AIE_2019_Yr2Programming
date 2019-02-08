#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject {
public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {};
	void applyForce(glm::vec2 force);
	void applyForceToActor(RigidBody* actor2, glm::vec2 force);
	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	glm::vec2 getPosition() { return m_Position; }
	glm::vec2 getVelocity() { return m_Velocity; }
	float getRotation() { return m_Rotation; }
	float getMass() { return m_Mass; }

	// ROCKET POWA
	void setMass(float newMass) { m_Mass = newMass; }

protected:

	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	float m_Mass;
	float m_Rotation;
};
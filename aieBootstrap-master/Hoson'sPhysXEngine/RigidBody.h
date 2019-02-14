#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject {
public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float drag, float angularDrag, float elasticity);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {};
	void applyForce(glm::vec2 force);
	void applyForceToActor(RigidBody* actor2, glm::vec2 force);

	glm::vec2 getPosition() { return m_Position; }
	glm::vec2 getVelocity() { return m_Velocity; }
	float getRotation() { return m_Rotation; }
	float getMass() { return m_Mass; }
	float getLinearDrag() { return m_LinearDrag; }
	float getAngularDrag() { return m_AngularDrag; }
	float getElasticity() { return m_Elasticity; }

	void setPosition(glm::vec2 newPos) { m_Position = newPos; }
	void setVelocity(glm::vec2 newVel) { m_Velocity = newVel; }
	void setMass(float newMass) { m_Mass = newMass; }
	float setLinearDrag(float newValue) { m_LinearDrag = newValue; }
	float setAngularDrag(float newValue) { m_AngularDrag = newValue; }

	void resolveCollision(RigidBody* other);

protected:

	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	float m_LinearDrag;

	float m_Rotation;
	// Add in the future: m_AngularVelocity;
	float m_AngularDrag;

	float m_Mass;
	float m_Elasticity;
};
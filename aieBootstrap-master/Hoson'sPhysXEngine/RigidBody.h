#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject {
public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float drag, float angularDrag, float elasticity);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {};
	void applyForce(glm::vec2 force, glm::vec2 position);

	glm::vec2 getPosition()const { return m_Position; }
	glm::vec2 getVelocity()const { return m_Velocity; }
	float getRotation()const { return m_Rotation; }
	float getMass()const { return m_Mass; }
	float getLinearDrag()const { return m_LinearDrag; }
	float getAngularDrag()const { return m_AngularDrag; }
	float getElasticity()const { return m_Elasticity; }
	float getAngularVelocity()const { return m_AngularVelocity; }
	float getMoment()const { return m_Moment; }

	void setPosition(glm::vec2 newPos) { m_Position = newPos; }
	void setVelocity(glm::vec2 newVel) { m_Velocity = newVel; }
	void setMass(float newMass) { m_Mass = newMass; }
	float setLinearDrag(float newValue) { m_LinearDrag = newValue; }
	float setAngularDrag(float newValue) { m_AngularDrag = newValue; }

	void resolveCollision(RigidBody* other, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

protected:

	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	float m_LinearDrag;

	float m_Rotation;
	float m_AngularVelocity;
	float m_AngularDrag;
	float m_Moment;

	float m_Mass;
	float m_Elasticity;
};
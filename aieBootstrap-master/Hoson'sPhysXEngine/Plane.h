#pragma once
#include "PhysicsObject.h"

class RigidBody;

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 normal, float distance);
	~Plane();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) {}
	virtual void debug() {}
	virtual void makeGizmo() const;
	virtual void resetPosition() {}

	glm::vec2 getNormal()	const { return m_Normal; }
	float getDistance()		const { return m_DistanceToOrigin; }

	void resolveCollision(glm::vec2 normal, RigidBody* other, glm::vec2 contact);

protected:

	glm::vec2 m_Normal;
	float m_DistanceToOrigin;
};
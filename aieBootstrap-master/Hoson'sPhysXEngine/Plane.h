#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 normal, float distance);
	~Plane();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) {}
	virtual void debug() {}
	virtual void makeGizmo();
	virtual void resetPosition() {}

	glm::vec2 getNormal() { return m_Normal; }
	float getDistance() { return m_DistanceToOrigin; }

protected:

	glm::vec2 m_Normal;
	float m_DistanceToOrigin;
};
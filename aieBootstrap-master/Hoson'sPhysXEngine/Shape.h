#pragma once
#include "RigidBody.h"

class Shape : public RigidBody
{
public:
	Shape(glm::vec2 pos, glm::vec2 vel, float mass, glm::vec2 extents, float drag, glm::vec4 colour, float elasticity = 1);
	~Shape() {}

	void makeGizmo();
	//void fixedUpdate(glm::vec2 gravity, float timeStep);
	void debug() {}

	glm::vec2 getExtents()	const { return m_Extents; }
	glm::vec4 getColour()	const { return m_Colour; }
	glm::vec2 getLocalX()	const { return m_LocalX; }
	glm::vec2 getLocalY()	const { return m_LocalY; }

	bool checkBoxCorners(const Shape& box, glm::vec2& contact, int& numContacts, glm::vec2& edgeNormal, glm::vec2& contactForce);

protected:

	glm::vec2 m_Extents;
	glm::vec4 m_Colour;

	glm::vec2 m_LocalX;
	glm::vec2 m_LocalY;
};
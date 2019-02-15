#pragma once
#include "RigidBody.h"

class Box : public RigidBody
{
public:
	Box(glm::vec2 pos, glm::vec2 vel, float mass, glm::vec2 extents, float drag, float angularDrag, glm::vec4 colour, float elasticity = 1);
	~Box() {}

	void makeGizmo();
	void fixedUpdate(glm::vec2 gravity, float timeStep);
	void debug() {}

	glm::vec2 getExtents()const { return m_Extents; }
	glm::vec4 getColour() { return m_Colour; }
	glm::vec2 getLocalX() { return m_LocalX; }
	glm::vec2 getLocalY() { return m_LocalY; }

	bool checkBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);

protected:

	glm::vec2 m_Extents;
	glm::vec4 m_Colour;

	glm::vec2 m_LocalX;
	glm::vec2 m_LocalY;
};
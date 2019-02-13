#pragma once
#include "RigidBody.h"

class Box : public RigidBody
{
public:
	Box(glm::vec2 pos, glm::vec2 vel, float mass, glm::vec2 extents, glm::vec4 colour);
	~Box() {}

	void makeGizmo();
	void debug() {}

	glm::vec2 getExtents() { return m_Extents; }
	glm::vec4 getColour() { return m_Colour; }

protected:

	glm::vec2 m_Extents;
	glm::vec4 m_Colour;
};
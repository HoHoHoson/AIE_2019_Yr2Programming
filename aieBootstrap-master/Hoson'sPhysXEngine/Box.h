#pragma once
#include "RigidBody.h"

class Box : public RigidBody
{
public: 

	Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, float drag, glm::vec4 colour, float elasticity = 1.0f);
	~Box();

	void makeGizmo();

	glm::vec2 getExtents()	const { return m_Extents; }
	glm::vec4 getColour()	const { return m_Colour; }

private:

	glm::vec2 m_Extents;
	glm::vec4 m_Colour;
};
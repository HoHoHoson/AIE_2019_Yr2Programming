#pragma once
#include "RigidBody.h"

class Shape : public RigidBody
{
public:
	Shape(glm::vec2 pos, glm::vec2 vel, float mass, float drag, int vertices, float radius, glm::vec4 colour, float elasticity = 1);
	~Shape() {}

	void makeGizmo();
	void debug() {}

protected:

	int m_Vertices;
	float m_Radius;
	glm::vec4 m_Colour;
};
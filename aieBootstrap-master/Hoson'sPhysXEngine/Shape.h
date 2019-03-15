#pragma once
#include "RigidBody.h"

using glm::vec2;
using glm::vec4;

class Shape : public RigidBody
{
public:
	Shape(vec2 pos, vec2 vel, float mass, float drag, int vertices, float radius, vec4 colour, float elasticity = 1);
	~Shape() {}

	void makeGizmo();
	void debug() {}

	/* Returns the total amount of vertices the polygon contains */
	int getVertices() { return m_Vertices; }
	float getRadius() { return m_Radius; }
	vec4 getColour() { return m_Colour; }

	vec2 getVertice(int n);

protected:

	int m_Vertices;
	float m_Radius;
	vec4 m_Colour;
};
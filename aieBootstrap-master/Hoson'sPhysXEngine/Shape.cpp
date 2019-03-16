#include "Shape.h"
#include <Gizmos.h>

#define _USE_MATH_DEFINES
#include <math.h>

Shape::Shape(glm::vec2 pos, glm::vec2 vel, float mass, float drag, int vertices, float radius, glm::vec4 colour, float elasticity) : RigidBody(SHAPE, pos, vel, mass, drag, elasticity)
{
	m_Vertices = vertices < 3 ? 0 : vertices;
	m_Radius = radius;
	m_Colour = colour;
}

void Shape::makeGizmo()
{
	if (m_Vertices == 0)
	{
		aie::Gizmos::add2DCircle(m_Position, m_Radius, 36, m_Colour);
	}
	else
	{
		for (int i = 0; i < m_Vertices; ++i)
		{
			glm::vec2 start(getVertice(i));
			glm::vec2 end(getVertice(i + 1));

			aie::Gizmos::add2DTri(m_Position, start, end, m_Colour);
		}
	}
}

/* Returns the nth vertice */
vec2 Shape::getVertice(int n)
{
	float nth = (n % m_Vertices + m_Vertices) % m_Vertices;
	assert(nth >= 0 && nth < m_Vertices);

	// May replace the "0" with the objects rotation in radians if I get to it
	float x = m_Radius * cosf((2 * M_PI) * (nth / m_Vertices) + 0) + m_Position.x;
	float y = m_Radius * sinf((2 * M_PI) * (nth / m_Vertices) + 0) + m_Position.y;

	return vec2(x, y);
}

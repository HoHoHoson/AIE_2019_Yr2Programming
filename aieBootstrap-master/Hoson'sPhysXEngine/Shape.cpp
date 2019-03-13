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
		for (float i = 0; i < m_Vertices; ++i)
		{
			glm::vec2 start(m_Radius * cosf((2 * M_PI) * (i / m_Vertices) + 0) + m_Position.x, m_Radius * sinf((2 * M_PI) * (i / m_Vertices) + 0) + m_Position.y);
			glm::vec2 end(m_Radius * cosf((2 * M_PI) * ((i + 1) / m_Vertices) + 0) + m_Position.x, m_Radius * sinf((2 * M_PI) * ((i + 1) / m_Vertices) + 0) + m_Position.y);

			aie::Gizmos::add2DTri(m_Position, start, end, m_Colour);
		}
	}
}

#include "Sphere.h"
#include <Gizmos.h>


Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float drag, float angularDrag, glm::vec4 colour, float elasticity) : RigidBody(SPHERE, position, velocity, 0, mass, drag, angularDrag, elasticity)
{
	m_Radius = radius;
	m_Colour = colour;

	m_Moment = 0.5f * m_Mass * m_Radius * m_Radius;
}

void Sphere::makeGizmo()
{
	glm::vec2 end = glm::vec2(std::cos(m_Rotation), std::sin(m_Rotation)) * m_Radius;

	aie::Gizmos::add2DCircle(m_Position, m_Radius, 36, m_Colour);
	aie::Gizmos::add2DLine(m_Position, m_Position + end, glm::vec4(1, 1, 1, 1));
}



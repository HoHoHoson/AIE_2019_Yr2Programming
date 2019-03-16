#include "Sphere.h"
#include <Gizmos.h>


Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float drag, glm::vec4 colour, float elasticity) : RigidBody(SHAPE, position, velocity, mass, drag, elasticity)
{
	m_Radius = radius;
	m_Colour = colour;
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_Position, m_Radius, 36, m_Colour);
}



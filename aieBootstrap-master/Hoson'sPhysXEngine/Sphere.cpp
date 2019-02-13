#include "Sphere.h"
#include <Gizmos.h>


Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : RigidBody(SPHERE, position, velocity, 0, mass)
{
	m_Radius = radius;
	m_Colour = colour;
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_Position, m_Radius, 36, m_Colour);
}



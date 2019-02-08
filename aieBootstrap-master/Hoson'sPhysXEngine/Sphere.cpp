#include "Sphere.h"
#include <Gizmos.h>


Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : RigidBody(SPHERE, position, velocity, 0, mass)
{
	m_Radius = radius;
	m_Colour = colour;
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_Position, m_Radius, 360, m_Colour);
}

bool Sphere::checkCollision(PhysicsObject* pOther)
{
	Sphere* sOther = dynamic_cast<Sphere*>(pOther);

	glm::vec2 displacement = m_Position - sOther->m_Position;
	float magnitudeSqr = powf(displacement.x, 2) + powf(displacement.y, 2);
	float totalRadiiSqr = powf(m_Radius + sOther->m_Radius, 2);

	return (magnitudeSqr <= totalRadiiSqr);
}


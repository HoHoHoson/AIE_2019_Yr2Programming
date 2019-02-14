#include "Plane.h"
#include "RigidBody.h"
#include <Gizmos.h>

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(PLANE)
{
	m_Normal = normal / sqrtf(powf(normal.x, 2) + powf(normal.y, 2));
	m_DistanceToOrigin = distance;
}

Plane::~Plane()
{
}

void Plane::makeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_Normal * m_DistanceToOrigin;
	glm::vec2 parallel(m_Normal.y, -m_Normal.x);
	glm::vec4 colour(1, 1, 1, 1);
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	
	aie::Gizmos::add2DLine(start, end, colour);
}

void Plane::resolveCollision(glm::vec2 normal, RigidBody * other)
{
	float j = glm::dot(-(1 + other->getElasticity()) * other->getVelocity(), normal) / (1 / other->getMass());
	glm::vec2 force = j * normal;

	other->applyForce(force);
}

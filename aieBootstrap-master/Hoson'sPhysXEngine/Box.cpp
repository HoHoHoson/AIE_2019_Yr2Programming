#include <Gizmos.h>
#include "Box.h"

Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, float drag, glm::vec4 colour, float elasticity) : RigidBody(BOX, position, velocity, mass, drag, elasticity)
{
	m_Extents = extents;
	m_Colour = colour;
}

Box::~Box()
{
}

void Box::makeGizmo()
{
	aie::Gizmos::add2DAABB(m_Position, m_Extents, m_Colour);
}

#include "Box.h"
#include <Gizmos.h>

Box::Box(glm::vec2 pos, glm::vec2 vel, float mass, glm::vec2 extents, float drag, float angularDrag, glm::vec4 colour, float elasticity) : RigidBody(BOX, pos, vel, 0, mass, drag, angularDrag, elasticity)
{
	m_Extents = extents;
	m_Colour = colour;
}

void Box::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_Position, m_Extents, m_Colour);
}

#include "Box.h"
#include <Gizmos.h>

Box::Box(glm::vec2 pos, glm::vec2 vel, float mass, glm::vec2 extents, float drag, float angularDrag, glm::vec4 colour, float elasticity) : RigidBody(BOX, pos, vel, 0, mass, drag, angularDrag, elasticity)
{
	m_Extents = extents;
	m_Colour = colour;
}

void Box::makeGizmo()
{
	glm::vec2 p1 = m_Position - m_LocalX * m_Extents.x - m_LocalY * m_Extents.y;
	glm::vec2 p2 = m_Position + m_LocalX * m_Extents.x - m_LocalY * m_Extents.y;
	glm::vec2 p3 = m_Position - m_LocalX * m_Extents.x + m_LocalY * m_Extents.y;
	glm::vec2 p4 = m_Position + m_LocalX * m_Extents.x + m_LocalY * m_Extents.y;
	aie::Gizmos::add2DTri(p1, p2, p3, m_Colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_Colour);
}

void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);

	float cs = cosf(m_Rotation);
	float sn = sinf(m_Rotation);
	m_LocalX = glm::normalize(glm::vec2(cs, sn));
	m_LocalY = glm::normalize(glm::vec2(-sn, cs));
}

bool Box::checkBoxCorners(const Box& box, glm::vec2 & contact, int & numContacts, float & pen, glm::vec2 & edgeNormal)
{
	float minX, maxX, minY, maxY;
	float boxW = box.getExtents().x * 2;
	float boxH = box.getExtents().y * 2;

	return false;
}

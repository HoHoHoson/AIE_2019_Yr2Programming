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
	aie::Gizmos::add2DTri(p1, p2, p4, m_Colour);
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
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);

	bool first = true;
	for (float x = -box.getExtents().x; x < boxW; x += boxW)
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH)
		{
			glm::vec2 p = box.getPosition() + x * box.m_LocalX + y * box.m_LocalY;
			glm::vec2 p0(glm::dot(p - m_Position, m_LocalX), glm::dot(p - m_Position, m_LocalY));

			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > minX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			if (p0.x >= -m_Extents.x && p0.x <= m_Extents.x && p0.y >= -m_Extents.y && p0.y <= m_Extents.y)
			{
				numLocalContacts++;
				localContact += p0;
			}

			first = false;
		}
	}

	if (maxX < -m_Extents.x || minX > m_Extents.x || maxY < -m_Extents.y || minY > m_Extents.y)
	{
		return false;
	}
	if (numLocalContacts == 0)
	{
		return false;
	}

	bool res = false;

	contact += m_Position + (localContact.x * m_LocalX + localContact.y * m_LocalY) / (float)numLocalContacts;
	numContacts++;

	float pen0 = m_Extents.x - minX;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = m_LocalX;
		pen = pen0;
		res = true;
	}

	pen0 = maxX + m_Extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -m_LocalX;
		pen = pen0;
		res = true;
	}

	pen0 = m_Extents.y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = m_LocalY;
		pen = pen0;
		res = true;
	}

	pen0 = m_Extents.y + m_Extents.y;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -m_LocalY;
		pen = pen0;
		res = true;
	}

	return res;
}

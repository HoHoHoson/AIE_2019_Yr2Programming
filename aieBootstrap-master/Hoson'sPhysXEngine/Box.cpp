#include "Box.h"
#include <Gizmos.h>

Box::Box(glm::vec2 pos, glm::vec2 vel, float mass, glm::vec2 extents, float drag, float angularDrag, glm::vec4 colour, float elasticity) : RigidBody(BOX, pos, vel, 0, mass, drag, angularDrag, elasticity)
{
	m_Extents = extents;
	m_Colour = colour;
	m_Moment = 1.0f / 12.0f * m_Mass * m_Extents.x * m_Extents.y;
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

bool Box::checkBoxCorners(const Box& box, glm::vec2 & contact, int & numContacts, glm::vec2 & edgeNormal, glm::vec2 & contactForce)
{
	float boxW = box.getExtents().x * 2;
	float boxH = box.getExtents().y * 2;
	float penetration = 0;

	for (float x = -box.getExtents().x; x < boxW; x += boxW)
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH)
		{
			glm::vec2 p = box.m_Position + x * box.m_LocalX + y * box.m_LocalY;
			glm::vec2 p0(glm::dot(p - m_Position, m_LocalX), glm::dot(p - m_Position, m_LocalY));
			float w2 = m_Extents.x;
			float h2 = m_Extents.y;

			if (p0.y < h2 && p0.y > -h2)
			{
				if (p0.x > 0 && p0.x < w2)
				{
					numContacts++;
					contact += m_Position + w2 * m_LocalX + p0.y * m_LocalY;
					edgeNormal = m_LocalX;
					penetration = w2 - p0.x;
				}
				if (p0.x < 0 && p0.x > -w2)
				{
					numContacts++;
					contact += m_Position - w2 * m_LocalX + p0.y * m_LocalY;
					edgeNormal = -m_LocalX;
					penetration = w2 + p0.x;
				}
			}

			if (p0.x < w2 && p0.x > -w2)
			{
				if (p0.y > 0 && p0.y < h2)
				{
					numContacts++;
					contact += m_Position + p0.x * m_LocalX + h2 * m_LocalY;
					float pen0 = h2 - p0.y;
					if (pen0 < penetration || penetration == 0)
					{
						penetration = pen0;
						edgeNormal = m_LocalY;
					}
				}
				if (p0.y < 0 && p0.y > -h2)
				{
					numContacts++;
					contact += m_Position + p0.x * m_LocalX - h2 * m_LocalY;
					float pen0 = h2 + p0.y;
					if (pen0 < penetration || penetration == 0)
					{
						penetration = pen0;
						edgeNormal = -m_LocalY;
					}
				}
			}
		}
	}

	contactForce = penetration * edgeNormal;
	return (penetration != 0);
}

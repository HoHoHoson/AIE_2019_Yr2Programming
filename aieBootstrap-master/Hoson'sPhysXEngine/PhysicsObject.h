#pragma once
#include <glm\ext.hpp>

enum ShapeType
{
	PLANE = 0,
	SPHERE,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
public:
	
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

	ShapeType getShapeID() { return m_ShapeID; }

protected:

	PhysicsObject(ShapeType a_shapeID) : m_ShapeID(a_shapeID) {}
	ShapeType m_ShapeID;
};
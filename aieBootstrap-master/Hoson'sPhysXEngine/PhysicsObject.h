#pragma once
#include <glm\ext.hpp>

enum ShapeType
{
	PLANE = 0,
	SPHERE,
	BOX
};

// This class is a stand in so that PhyscicsScene works properly
class PhysicsObject
{
public:
	
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

protected:

	PhysicsObject(ShapeType a_shapeID) : m_ShapeID(a_shapeID) {}
	ShapeType m_ShapeID;
};
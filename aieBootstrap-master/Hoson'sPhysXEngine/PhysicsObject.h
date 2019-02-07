#pragma once
#include <glm\ext.hpp>

// This class is a stand in so that PhyscicsScene works properly
class PhysicsObject
{
public:
	
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

protected:

	PhysicsObject() {}

};
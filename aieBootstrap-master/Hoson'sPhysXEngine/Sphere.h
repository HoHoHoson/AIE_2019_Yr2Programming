#pragma once
#include "RigidBody.h"

 class Sphere : public RigidBody
 {
 public:

	 Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
	 ~Sphere() {}

	 virtual void makeGizmo();

	 float getRadius() { return m_Radius; }
	 glm::vec4 getColour() { return m_Colour; }
	 void debug() {}

 protected:

	 float m_Radius;
	 glm::vec4 m_Colour;
};
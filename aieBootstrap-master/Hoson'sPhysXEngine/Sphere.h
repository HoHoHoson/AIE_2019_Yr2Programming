#pragma once
#include "RigidBody.h"

 class Sphere : public RigidBody
 {
 public:

	 Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float drag, float angularDrag, glm::vec4 colour, float elasticity = 1);
	 ~Sphere() {}

	 void makeGizmo() const;

	 float getRadius()		const { return m_Radius; }
	 glm::vec4 getColour()	const { return m_Colour; }
	 void debug() {}

 protected:

	 float m_Radius;
	 glm::vec4 m_Colour;
};
#pragma once
#include <vector>
#include <glm\ext.hpp>


class PhysicsObject;

class PhysicsScene
{
public:

	PhysicsScene();
	~PhysicsScene();

	bool addActor(PhysicsObject*);
	bool removeActor(PhysicsObject*);
	void update(float deltaTime);
	void updateGizmos();

	void setGravity(const glm::vec2 gravity) {}

protected:

	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;

};
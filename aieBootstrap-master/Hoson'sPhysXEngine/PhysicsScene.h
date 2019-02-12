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
	void debugScene();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	void setScreenDimensions(int sWidth, int sHeight) { m_ScreenWidth = sWidth; m_ScreenHeight = sHeight; }

	//// ROCKET POWA
	/*PhysicsObject* getPlayer() { return m_actors.front(); }*/

protected:

	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;

	int m_ScreenWidth;
	int m_ScreenHeight;
};
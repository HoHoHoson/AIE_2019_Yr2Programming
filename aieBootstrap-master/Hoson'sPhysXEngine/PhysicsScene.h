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
	void updateGizmos() const;
	void debugScene();

	void setGravity(const glm::vec2& gravity) { m_gravity = gravity; }
	void setScreenDimensions(int sWidth, int sHeight) { m_ScreenWidth = sWidth; m_ScreenHeight = sHeight; }

	void checkForCollision() const;

	static bool planeToPlane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool planeToShape(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool shapeToPlane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool shapeToShape(PhysicsObject* obj1, PhysicsObject* obj2);

	static bool planeToSphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool planeToBox(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool sphereToPlane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool sphereToSphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool sphereToBox(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool boxToPlane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool boxToSphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool boxToBox(PhysicsObject* obj1, PhysicsObject* obj2);

protected:

	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;

	int m_ScreenWidth;
	int m_ScreenHeight;

private:

	static void setSATmaxmin(const vec2& axis, Shape* s, float& min, float& max, vec2* pMin, vec2* pMax);
	static bool isSATintersect(Shape* mainS, Shape* secondaryS, vec2& s1Min, vec2& s1Max, vec2& s2Min, vec2& s2Max, vec2& axis, float& penetration);
};
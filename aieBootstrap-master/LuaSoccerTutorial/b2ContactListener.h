#pragma once
#include <Box2D/Box2D.h>

class MyContactListener : public b2ContactListener
{
	void beginContact(b2Contact* contact);
	void endContact(b2Contact* contact);
};
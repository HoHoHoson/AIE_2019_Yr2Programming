#pragma once
#include "BaseLuaObject.h"
#include <Box2D/Box2D.h>

namespace aie
{
	class Renderer2D;
};
class Texture;

class Agent : public BaseLuaObject
{
public:

	Agent(b2World* pWorld, b2Vec2 position);
	~Agent() {};

	void update(float dt);
	void draw(aie::Renderer2D* sprite_batch);

	const b2Vec2& getPosition() { return m_b2_body->GetPosition(); }
	void applyForce(const b2Vec2& force);
	void applyRotation(const float force);
	void onCollision(b2Vec2 otherPosition);

	static int luaGetPosition(lua_State* pState);
	static int luaApplyForce(lua_State* pState);
	static int luaApplyRotation(lua_State* pState);
	static int luaGetAngle(lua_State* pState);

	b2Body* m_b2_body;
	Texture* m_pTexture;
};
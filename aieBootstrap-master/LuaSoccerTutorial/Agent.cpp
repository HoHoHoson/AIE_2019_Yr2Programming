#include "Agent.h"
#include "b2UserData.h"
#include <Texture.h>
#include <Renderer2D.h>

Agent::Agent(b2World * pWorld, b2Vec2 position)
{
	b2BodyDef body_def;
	body_def.type = b2_dynamicBody;
	body_def.position = position;
	m_b2_body = pWorld->CreateBody(&body_def);

	// Define another box shape for our dynamic body
	b2PolygonShape player_dynamic_box;
	player_dynamic_box.SetAsBox(12.0f, 12.0f);
	// define the dynamic body fixture
	b2FixtureDef player_fixture_def;
	player_fixture_def.shape = &player_dynamic_box;
	// Set the box density to be non-zero, so it will be dynamic
	player_fixture_def.density = 1.0f;
	// Overrid the default friction
	player_fixture_def.friction = 0.3f;
	// Add the shape to the body
	m_b2_body->CreateFixture(&player_fixture_def);

	registerLuaFunction("GetPosition", luaGetPosition);
	registerLuaFunction("ApplyForce", luaApplyForce);
	registerLuaFunction("ApplyRotation", luaApplyRotation);
}

int Agent::luaGetPosition(lua_State* pState)
{
	Agent* agent = (Agent*)getPointerVar(pState, "self");

	b2Vec2 pos = agent->getPosition();
	agent->pushFloat(pos.x);
	agent->pushFloat(pos.x);

	return 2;
}

int Agent::luaApplyForce(lua_State* pState)
{
	Agent* agent = (Agent*)getPointerVar(pState, "self");

	b2Vec2 force;
	force.x = agent->popFloat();
	force.y = agent->popFloat();
	agent->applyForce(force);
	
	return 0;
}

int Agent::luaApplyRotation(lua_State* pState)
{
	Agent* agent = (Agent*)getPointerVar(pState, "self");
	float rotation = agent->popFloat();
	agent->applyRotation(rotation);

	return 0;
}

void Agent::applyForce(const b2Vec2& force)
{
	m_b2_body->SetLinearVelocity(force);
}

void Agent::applyRotation(const float force)
{
	m_b2_body->SetAngularVelocity(force);
}

void Agent::update(float dt)
{
	pushFunction("Update");
	pushFloat(dt);
	callFunction(1, 0);
}

void Agent::draw(aie::Renderer2D* renderer)
{
	b2Vec2 position = m_b2_body->GetPosition();
	float angle = m_b2_body->GetAngle();

	renderer->drawSprite(nullptr, position.x, position.y, 24, 24, angle);
}
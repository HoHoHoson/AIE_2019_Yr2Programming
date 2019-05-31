#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Agent.h"
#include <list>

class b2World;
class b2Body;
class Agent;
class MyContactListener;

class LuaSoccerTutorialApp : public aie::Application 
{
	static LuaSoccerTutorialApp* m_pInstance;
	LuaSoccerTutorialApp();

public:

	virtual ~LuaSoccerTutorialApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void doBallHit(void* otherObjData);

	static LuaSoccerTutorialApp* instance();
	static int luaGetBallPosition(lua_State* pState);
	static int luaGetGoalPosition(lua_State* pState);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	aie::Texture*		m_ball_texture;
	b2World*			m_world;
	b2Body*				m_soccer_ball;
	b2Body*				m_left_goal;
	std::list<Agent*>	m_players;

	bool				m_is_gameover;

	MyContactListener* m_listener_instance;
};
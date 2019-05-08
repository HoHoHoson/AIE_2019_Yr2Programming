#pragma once

#include "Application.h"
#include "Renderer2D.h"

class b2World;
class b2Body;

class LuaSoccerTutorialApp : public aie::Application {
public:

	LuaSoccerTutorialApp();
	virtual ~LuaSoccerTutorialApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	aie::Texture*		m_ball_texture;
	b2World*			m_world;
	b2Body*				m_soccer_ball;
};
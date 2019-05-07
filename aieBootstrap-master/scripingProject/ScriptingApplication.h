#pragma once

#include "Application.h"
#include "Renderer2D.h"


class Character;
class ScriptedCharacterController;

class b2World;
class b2Body;

class ScriptingApplication : public aie::Application {
public:

	ScriptingApplication();
	virtual ~ScriptingApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_background;

	Character*			m_character;
	ScriptedCharacterController* m_controller;
	float m_cameraX, m_cameraY;
	float m_timer;
};

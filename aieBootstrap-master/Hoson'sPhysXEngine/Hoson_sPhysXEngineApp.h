#pragma once

#include "Application.h"
#include "Renderer2D.h"

class PhysicsScene;

class Hoson_sPhysXEngineApp : public aie::Application {
public:

	Hoson_sPhysXEngineApp();
	virtual ~Hoson_sPhysXEngineApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_PhysicsScene;

	// ROCKET POWA
	float fuelWeight = 0.1f;
	float fuelVelocity = -100;
	float rocketPropellant = 0;
	float fuelConsumption = 0.1f;
};
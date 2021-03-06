#include "Hoson_sPhysXEngineApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "PhysicsScene.h"
#include "Plane.h"
#include "Shape.h"
#include <Gizmos.h>
#define _USE_MATH_DEFINES
#include <math.h>

Hoson_sPhysXEngineApp::Hoson_sPhysXEngineApp()
{

}

Hoson_sPhysXEngineApp::~Hoson_sPhysXEngineApp() {
	
}

bool Hoson_sPhysXEngineApp::startup() 
{
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_PhysicsScene = new PhysicsScene();
	m_PhysicsScene->setGravity(glm::vec2(0, -10));

	for (int i = 0; i < 13; ++i)
	{
		Shape* shape = new Shape(glm::vec2(-80 + i * 10, 0), glm::vec2(0, 50), 1.0f, 0.0f, i, 5.0f, glm::vec4((float)i / 12, (float)i / 12, 1, 1), 0.9f);
		m_PhysicsScene->addActor(shape);
	}

	Plane* plane1 = new Plane(glm::vec2(1, 10), -50);
	m_PhysicsScene->addActor(plane1);
	Plane* plane2 = new Plane(glm::vec2(1, -10), -50);
	m_PhysicsScene->addActor(plane2);
	Plane* plane3 = new Plane(glm::vec2(10, 1), -90);
	m_PhysicsScene->addActor(plane3);
	Plane* plane4 = new Plane(glm::vec2(-10, 1), -90);
	m_PhysicsScene->addActor(plane4);

	return true;
}

void Hoson_sPhysXEngineApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void Hoson_sPhysXEngineApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	// Gizmos are cleared and created each frame to simulate movement of ingame objects
	aie::Gizmos::clear();

	// Commented updateGizmos to demonstrate descrete simulation comparison with numerical intergration
	m_PhysicsScene->update(deltaTime);
	m_PhysicsScene->updateGizmos();
	m_PhysicsScene->checkForCollision();
	
	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Hoson_sPhysXEngineApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	
	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}
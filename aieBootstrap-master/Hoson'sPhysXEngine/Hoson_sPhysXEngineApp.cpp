#include "Hoson_sPhysXEngineApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include <Gizmos.h>

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
	m_PhysicsScene->setScreenDimensions(getWindowWidth(), getWindowHeight());
	m_PhysicsScene->setGravity(glm::vec2(0, -9.8f));

	//Sphere* ball1 = new Sphere(glm::vec2(20, 0), glm::vec2(-5, 0), 10.0f, 4, glm::vec4(1, 0, 0, 1));
	//Sphere* ball2 = new Sphere(glm::vec2(-20, 0), glm::vec2(10, 0), 10.0f, 4, glm::vec4(0, 1, 0, 1));

	//m_PhysicsScene->addActor(ball1);
	//m_PhysicsScene->addActor(ball2);

	// ROCKET POWA
	player = new Sphere(glm::vec2(0, 0), glm::vec2(0, 0), 10.0f, 5, glm::vec4(1, 1, 1, 1));
	m_PhysicsScene->addActor(player);

	return true;
}

void Hoson_sPhysXEngineApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void Hoson_sPhysXEngineApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	//
	// ROCKET POWA
	rocketPropellant += deltaTime;

	if (player->getMass() < 0)
	{
     	player->setMass(1.0f);
	}

  	if (player->getMass() > 1.0f && input->isKeyDown(aie::INPUT_KEY_SPACE) && rocketPropellant > fuelRate)
	{
		float decreaseMass = player->getMass() - fuelWeight;
		player->setMass(decreaseMass);         

		Sphere *gas;
   		gas = new Sphere(player->getPosition(), glm::vec2(0, fuelVelocity), fuelWeight, 1, glm::vec4(0, 0, 1, 1)); 
		gas->applyForceToActor(player, gas->getMass() * gas->getVelocity());
		m_PhysicsScene->addActor(gas);

		rocketPropellant = 0;
	}
	// ROCKET POWA
	//

	// Gizmos are cleared and created each frame to simulate movement of ingame objects
	aie::Gizmos::clear();

	m_PhysicsScene->update(deltaTime);
	m_PhysicsScene->updateGizmos();
	
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
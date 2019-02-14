#include "Hoson_sPhysXEngineApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
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
	m_PhysicsScene->setScreenDimensions(getWindowWidth(), getWindowHeight());
	m_PhysicsScene->setGravity(glm::vec2(0, -10));

	Sphere* spaceCore1 = new Sphere(glm::vec2(0, 20), glm::vec2(0, -5), 10, 10, 0, 0, glm::vec4(1, 0, 0, 1), 1.1f);
	m_PhysicsScene->addActor(spaceCore1);
	Sphere* spaceCore2 = new Sphere(glm::vec2(20, 0), glm::vec2(0, -5), 1.0f, 10, 0, 0, glm::vec4(0, 1, 0, 1), 1.1f);
	m_PhysicsScene->addActor(spaceCore2);
	Sphere* spaceCore3 = new Sphere(glm::vec2(-20, 0), glm::vec2(0, -5), 1.0f, 10, 0, 0, glm::vec4(0, 0, 1, 1), 1.1f);
	m_PhysicsScene->addActor(spaceCore3);

	Plane* plane1 = new Plane(glm::vec2(1, 10), 50);
	m_PhysicsScene->addActor(plane1);
	Plane* plane2 = new Plane(glm::vec2(1, 10), -50);
	m_PhysicsScene->addActor(plane2);
	Plane* plane3 = new Plane(glm::vec2(10, 1), 90);
	m_PhysicsScene->addActor(plane3);
	Plane* plane4 = new Plane(glm::vec2(10, 1), -90);
	m_PhysicsScene->addActor(plane4);
	//Box* companionCube = new Box(glm::vec2(0, 40), glm::vec2(0, -10), 1, glm::vec2(100, 5), glm::vec4(0, 1, 0, 1));
	//m_PhysicsScene->addActor(companionCube);

	/// ROCKET POWA
	//player = new Sphere(glm::vec2(0, 0), glm::vec2(0, 0), 10.0f, 5, glm::vec4(1, 1, 1, 1));
	//m_PhysicsScene->addActor(player);

	/// Code below is for the projectiles demo
	//float xSpeed = cosf(45.0f / 180 * M_PI) * 35;
	//float ySpeed = sinf(45.0f / 180 * M_PI) * 35;
	//Sphere* testProjectile = new Sphere(glm::vec2(-40, 0), glm::vec2(xSpeed, ySpeed), 1, 1, glm::vec4(1, 1, 1, 1));
	//m_PhysicsScene->addActor(testProjectile);
	//projectileArcDemo(glm::vec2(-40, 0), 45, 35, -10);

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
	//// ROCKET POWA ////
	//rocketPropellant += deltaTime;

	//if (player->getMass() < 0)
	//{
	//    	player->setMass(1.0f);
	//}

 // 	if (player->getMass() > 1.0f && input->isKeyDown(aie::INPUT_KEY_SPACE) && rocketPropellant > fuelRate)
	//{
	//	float decreaseMass = player->getMass() - fuelWeight;
	//	player->setMass(decreaseMass);         

	//	Sphere *gas;
 //  		gas = new Sphere(player->getPosition(), glm::vec2(0, fuelVelocity), fuelWeight, 1, glm::vec4(0, 0, 1, 1)); 
	//	gas->applyForceToActor(player, gas->getMass() * gas->getVelocity());
	//	m_PhysicsScene->addActor(gas);

	//	rocketPropellant = 0;
	//}
	//// ROCKET POWA ////
	//

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

void Hoson_sPhysXEngineApp::projectileArcDemo(glm::vec2 startPos, float inclination, float speed, float gravity)
{
	float t = 0;
	float tStep = 0.5f;
	float radius = 1.0f;
	int segments = 12;
	float xSpeed = cosf(inclination / 180 * M_PI) * speed;
	float ySpeed = sinf(inclination / 180 * M_PI) * speed;
	glm::vec2 pos = startPos;
	glm::vec4 colour(1, 1, 0, 1);

	while (t <= 5)
	{
		pos.x = startPos.x + xSpeed * t;
		pos.y = startPos.y + ySpeed * t + 0.5 * gravity * powf(t, 2);

		aie::Gizmos::add2DCircle(pos, radius, segments, colour);
		t += tStep;
	}
}


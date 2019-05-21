#include "LuaSoccerTutorialApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Agent.h"
#include "b2UserData.h"

#include <Box2D/Box2D.h>

LuaSoccerTutorialApp::LuaSoccerTutorialApp() : m_is_gameover(false)
{
}

LuaSoccerTutorialApp::~LuaSoccerTutorialApp() {

}

bool LuaSoccerTutorialApp::startup() 
{
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_ball_texture = new aie::Texture("../bin/textures/ball.png");

	b2Vec2 gravity(0.0f, 0.0f);
	m_world = new b2World(gravity);

	const float32 k_restitution = 0.4f;

	b2Body* ground;
	{
		b2BodyDef bd;
		bd.position.Set(0.0f, 0.0f);
		ground = m_world->CreateBody(&bd);

		b2EdgeShape shape;
		b2FixtureDef sd;
		sd.shape = &shape;
		sd.density = 0.0f;
		sd.restitution = k_restitution;

		// Bounds for keeping physics objects on screen
		// Left
		shape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, 720.0f));
		ground->CreateFixture(&sd);

		// Right
		shape.Set(b2Vec2(1280.0f, 0.0f), b2Vec2(1280.0f, 720.0f));
		ground->CreateFixture(&sd);

		// Top
		shape.Set(b2Vec2(0.0f, 720.0f), b2Vec2(1280.0f, 720.0f));
		ground->CreateFixture(&sd);

		// Bottom
		shape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(1280.0f, 0.0f));
		ground->CreateFixture(&sd);
	}

	// Left goal
	{
		b2BodyDef bd;
		bd.position.Set(100.0f, 360.0f);
		m_left_goal = m_world->CreateBody(&bd);

		b2EdgeShape shape;
		b2FixtureDef sd;
		sd.shape = &shape;
		sd.density = 0.0f;
		sd.restitution = k_restitution;

		// Left vertical
		shape.Set(b2Vec2(0.0f, -50.0f), b2Vec2(0.0f, 50.0f));

		m_left_goal->CreateFixture(&sd);
		m_left_goal->SetUserData(new b2UserData(GOAL));
	}

	// Create a dynamic body for the soccer ball
	b2BodyDef body_def;
	body_def.type = b2_dynamicBody;
	body_def.position.Set(640.0f, 360.0f);
	m_soccer_ball = m_world->CreateBody(&body_def);

	// Define shape for the soccer ball, in this case, a box
	b2PolygonShape dynamic_box;
	dynamic_box.SetAsBox(24.0f, 24.0f);

	// Define dynamic body fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &dynamic_box;
	// Set box density to non zero, so it'll be dynamic
	fixture_def.density = 1.0f;
	// Friction value
	fixture_def.friction = 0.3f;

	// Attach shape to the object
	m_soccer_ball->CreateFixture(&fixture_def);
	m_soccer_ball->SetLinearVelocity(b2Vec2(10, -15));
	m_soccer_ball->SetUserData(new b2UserData(EntityType::BALL));

	Agent* agent = new Agent(m_world, b2Vec2(100, 100));
	if (agent->loadScript("../bin/scripts/basicLuaScript.lua") == false)
	{
		delete agent;
		return false;
	}

	agent->m_b2_body->SetUserData(new b2UserData(EntityType::AGENT, (void*)agent));
	m_players.push_back(agent);

	return true;
}

void LuaSoccerTutorialApp::shutdown() 
{
	delete m_font;
	delete m_2dRenderer;

	m_world->DestroyBody(m_soccer_ball);
	delete m_world;
	delete m_ball_texture;
}

void LuaSoccerTutorialApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	// Prepare for simulation. Typically we use a time step of 1/60 of a 
	// second (60Hz) and 10 iterations. This provides a high quality simulation 
	// in most game scenarios.
	static float32 time_step = 1.0f / 60.0f;
	static int32 velocity_iterations = 6;
	static int32 position_iterations = 2;

	// Instruct the world to perform a single step of simulation 
	// It is generall best to keep the time step and interations fixed
	m_world->Step(time_step, velocity_iterations, position_iterations);

	for (auto agent : m_players) 
		agent->update(deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void LuaSoccerTutorialApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	b2Vec2 position = m_soccer_ball->GetPosition();
	float angle = m_soccer_ball->GetAngle();

	m_2dRenderer->drawSprite(m_ball_texture, position.x, position.y, 0, 0, angle);

	for (auto agent : m_players)
		agent->draw(m_2dRenderer);

	position = m_left_goal->GetPosition();
	m_2dRenderer->setRenderColour(1, 0, 0, 1);
	m_2dRenderer->drawSprite(nullptr, position.x, position.y, 10, 100);
	m_2dRenderer->setRenderColour(1, 1, 1, 1);

	if (m_is_gameover) 
	{
		m_2dRenderer->drawText(m_font, "Gooooooooooooooooooooal!!!1", 200, 700);
	}

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}
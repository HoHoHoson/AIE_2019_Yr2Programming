#include "ComputerGraphics.h"
#include "FreeCamera.h"
#include <Gizmos.h>

#include <iostream>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

ComputerGraphics::ComputerGraphics()
{
}

ComputerGraphics::~ComputerGraphics()
{
}

bool ComputerGraphics::startUp()
{
	if (glfwInit() == false)
		return false;

	m_Window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (m_Window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_Window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		return false;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	Gizmos::create(255U, 255U, 65535U, 65535U);

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	m_Camera = new FreeCamera(m_Window, 5.0f);

	return true;
}

void ComputerGraphics::shutDown()
{
	Gizmos::destroy();
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

bool ComputerGraphics::update(float deltaTime)
{
	if (glfwWindowShouldClose(m_Window) == true || glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	m_Camera->update(deltaTime);

	return true;
}

void ComputerGraphics::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::clear();

	// DRAW BEGIN

	Gizmos::addTransform(mat4(1));

	vec4 white(1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), white);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), white);
	}

	Gizmos::addSphere(vec3(0, 0, 0), 1, 10, 10, vec4(0.97f, 0.71f, 0.22f, 0.5f));

	// DRAW END

	Gizmos::draw(m_Camera->getProjectionView());
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}
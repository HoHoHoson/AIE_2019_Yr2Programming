#include "ComputerGraphics.h"
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

	window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	Gizmos::create(255U, 255U, 65535U, 65535U);

	view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.0f, 0.1f, 1000.0f);

	return true;
}

void ComputerGraphics::shutDown()
{
	Gizmos::destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool ComputerGraphics::update()
{
	if (glfwWindowShouldClose(window) == true || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

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

	Gizmos::draw(projection * view);
	glfwSwapBuffers(window);
	glfwPollEvents();
}
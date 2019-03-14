#pragma once
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class ComputerGraphics
{
public:

	ComputerGraphics();
	virtual ~ComputerGraphics();

	virtual bool startUp();
	virtual void shutDown();

	virtual bool update();
	virtual void draw();

protected:

	GLFWwindow* window;
	glm::mat4 view;
	glm::mat4 projection;
};
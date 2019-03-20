#pragma once
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera;

class ComputerGraphics
{
public:

	ComputerGraphics();
	virtual ~ComputerGraphics();

	virtual bool startUp();
	virtual void shutDown();

	virtual bool update(float deltaTime);
	virtual void draw();

protected:

	GLFWwindow* m_Window;
	Camera* m_Camera;
};
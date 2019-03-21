#include "FreeCamera.h"
#include <iostream>

FreeCamera::FreeCamera(GLFWwindow* window, float speed) : Camera(window)
{
	m_Speed = speed;

	m_StartMouse = true;
	m_LastPosX = 0;
	m_LastPosY = 0;
	m_Sensitivity = 0.05f;
}

FreeCamera::~FreeCamera()
{
	
}

void FreeCamera::update(float deltaTime)
{
	// Mouse input processing
	double* posX = new double();
	double* posY = new double();
	glfwGetCursorPos(m_CamWindow, posX, posY);

	if (m_StartMouse)
	{
		m_LastPosX = (float)*posX;
		m_LastPosY = (float)*posY;
		m_StartMouse = false;
	}

	if (m_LastPosX != *posX || m_LastPosY != *posY)
	{
		float deltaX = (float)*posX - m_LastPosX;
		float deltaY = (float)*posY - m_LastPosY;
		m_LastPosX = (float)*posX;
		m_LastPosY = (float)*posY;

		deltaX *= m_Sensitivity;
		deltaY *= m_Sensitivity;

		m_Yaw += deltaX;
		m_Pitch += deltaY;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		vec3 front;
		front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
		front.y = sin(glm::radians(m_Pitch));
		front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
		front = glm::normalize(front);
		vec3 right = glm::normalize(glm::cross(front, vec3(0, 1, 0)));
		vec3 m_Up = glm::normalize(glm::cross(right, front));

		setLookAt(vec3(m_WorldTransform[3]), vec3(m_WorldTransform[3]) - front, m_Up);
	}

	delete posX;
	delete posY;

	// Keyboard input processing
	vec3 totalDir(0);

	if (glfwGetKey(m_CamWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		totalDir -= vec3(m_WorldTransform[2]);
	}
	if (glfwGetKey(m_CamWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		totalDir += vec3(m_WorldTransform[2]);
	}
	if (glfwGetKey(m_CamWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		totalDir -= vec3(m_WorldTransform[0]);
	}
	if (glfwGetKey(m_CamWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		totalDir += vec3(m_WorldTransform[0]);
	}

	if (totalDir != vec3(0))
	{
		totalDir = glm::normalize(totalDir);
		setPosition(vec3(m_WorldTransform[3]) + totalDir * m_Speed * deltaTime);
	}
}

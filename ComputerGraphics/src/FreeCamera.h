#pragma once
#include "Camera.h"

class GLFWwindow;

class FreeCamera : public Camera
{
public:

	FreeCamera(GLFWwindow* window, float speed);
	~FreeCamera();

	void update(float deltaTime);

protected:

	float	m_Speed;
	vec3	m_Up;

private:

	bool    m_StartMouse;
	float   m_LastPosX;
	float	m_LastPosY;
	float	m_Yaw;
	float	m_Pitch;
	float	m_Sensitivity;
};
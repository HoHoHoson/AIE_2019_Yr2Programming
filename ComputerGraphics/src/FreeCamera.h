#pragma once
#include "Camera.h"

class GLFWwindow;

/* Free Camera class, derived from the Camera class */
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
	float	m_Sensitivity;
};
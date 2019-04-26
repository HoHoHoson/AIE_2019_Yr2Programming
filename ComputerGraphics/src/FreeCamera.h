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

	// The movement speed of the free camera
	float	m_Speed;
	// Stores the UP axis of the camera
	vec3	m_Up;

private:

	// Value to check if the mouse variables have been initiated
	bool    m_StartMouse;
	// Stores the position of the mouse cursor during the last frame, for calculating the distanced moved
	float   m_LastPosX;
	float	m_LastPosY;
	// Stores the mouse sensitivity 
	float	m_Sensitivity;
};
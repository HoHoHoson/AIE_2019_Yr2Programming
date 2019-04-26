#pragma once
#include <glm/ext.hpp>
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;

/*
	Parent class that has the basic functions and variables vital to every camera type.
	Should be derived from to create specific camera types
*/
class Camera
{
public:

	Camera(GLFWwindow* window);
	~Camera();

	virtual void update(float deltaTime) = 0;

	/*
		@brief Sets the projection transform of the camera
		@param Field of view
		@param Aspect ratio
		@param Near value
		@param Far value
	*/
	void setPerspective(float fov, float aspectRatio, float n, float f);
	/*
		@brief Sets the view transform of the camera
		@param Vector3 of the cameras position
		@param Vector3 of where the camera is looking at
		@param Vector3 of the cameras up axis
	*/
	void setLookAt(vec3 from, vec3 to, vec3 up);
	/*
		@brief Sets the cameras position to a new one
		@param Vector3 of the new position
	*/
	void setPosition(vec3 pos);

	/* Returns the camera's view transform */
	mat4 getView() { return m_ViewTransform; }
	/* Returns the camera's projection transform */
	mat4 getProjection() { return m_ProjectionTransform; }
	/* Returns the camera's projection-view transform */
	mat4 getProjectionView() { return m_ProjectionViewTransform; }
	/* Returns the camera's world transform */
	mat4 getWorldTransform() { return m_WorldTransform; }

protected:

	// Stores the reference of the main GLFWindow
	GLFWwindow* m_CamWindow;

	// The multiple camera transforms
	mat4 m_WorldTransform;
	mat4 m_ViewTransform;
	mat4 m_ProjectionTransform;
	mat4 m_ProjectionViewTransform;

	// The Yaw and Pitch values
	float m_Yaw;
	float m_Pitch;

	/* Updates the projection view transform */
	void updateProjectionViewTransform();
};
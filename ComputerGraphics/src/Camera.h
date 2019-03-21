#pragma once
#include <glm/ext.hpp>
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera
{
public:

	Camera(GLFWwindow* window);
	~Camera();

	virtual void update(float deltaTime) = 0;

	void setPerspective(float fov, float aspectRatio, float n, float f);
	void setLookAt(vec3 from, vec3 to, vec3 up);
	void setPosition(vec3 pos);

	mat4 getView() { return m_ViewTransform; }
	mat4 getProjection() { return m_ProjectionTransform; }
	mat4 getProjectionView() { return m_ProjectionViewTransform; }
	mat4 getWorldTransform() { return m_WorldTransform; }

protected:

	GLFWwindow* m_CamWindow;

	mat4 m_WorldTransform;
	mat4 m_ViewTransform;
	mat4 m_ProjectionTransform;
	mat4 m_ProjectionViewTransform;

	float m_Yaw;
	float m_Pitch;

	void updateProjectionViewTransform();
};
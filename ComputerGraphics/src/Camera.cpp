#include "Camera.h"

Camera::Camera(GLFWwindow* window)
{
	m_CamWindow = window;

	setLookAt(vec3(0), vec3(1, 0, 0), vec3(0, 1, 0));
	setPerspective(glm::pi<float>() * 0.25f, 16 / 9.0f, 0.1f, 1000.0f);
}

Camera::~Camera()
{
}

void Camera::setPerspective(float fov, float aspectRatio, float n, float f)
{
	m_ProjectionTransform = glm::perspective(fov, aspectRatio, n, f);

	updateProjectionViewTransform();
}

void Camera::setLookAt(vec3 from, vec3 to, vec3 up)
{
	m_ViewTransform = glm::lookAt(from, to, up);
	m_WorldTransform = glm::inverse(m_ViewTransform);

	updateProjectionViewTransform();
}

void Camera::setPosition(vec3 pos)
{
	m_WorldTransform[3] = vec4(pos, m_WorldTransform[3].w);
	m_ViewTransform = glm::inverse(m_WorldTransform);

	updateProjectionViewTransform();
}

void Camera::updateProjectionViewTransform()
{
	m_ProjectionViewTransform = m_ProjectionTransform * m_ViewTransform;
}

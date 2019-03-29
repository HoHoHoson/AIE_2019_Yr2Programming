#pragma once
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Gizmos.h>

#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

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

	aie::Texture m_GridTexture;

	aie::ShaderProgram	m_GeoShader;
	Mesh				m_GeoMesh;
	mat4				m_GeoTransform;

	aie::ShaderProgram	m_ModelShader;
	aie::OBJMesh		m_ModelMesh;
	mat4				m_ModelTransform;

	struct Light
	{
		vec3 direction;
		vec3 diffuse;
		vec3 specular;
	};
	Light				m_Light1;
	Light				m_Light2;

	vec3				m_AmbientLight;
};
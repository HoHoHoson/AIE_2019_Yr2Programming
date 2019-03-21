#pragma once
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"

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

	aie::ShaderProgram	m_Shader;
	Mesh				m_CubeMesh;
	glm::mat4			m_CubeTransform;

	aie::ShaderProgram	m_ModelShader;
	aie::OBJMesh		m_ModelMesh;
	glm::mat4			m_ModelTransform;
};
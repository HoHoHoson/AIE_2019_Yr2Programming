#include "ComputerGraphics.h"
#include "FreeCamera.h"
#include <Gizmos.h>

#include <iostream>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

ComputerGraphics::ComputerGraphics()
{
}

ComputerGraphics::~ComputerGraphics()
{
}

bool ComputerGraphics::startUp()
{
	if (glfwInit() == false)
		return false;

	m_Window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (m_Window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_Window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		return false;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	Gizmos::create(255U, 255U, 65535U, 65535U);

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	m_Camera = new FreeCamera(m_Window, 5.0f);
	m_Camera->setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));

	// Rendering Geometry
	//m_Shader.loadShader(aie::eShaderStage::VERTEX, "../shaders/simple.vert");
	//m_Shader.loadShader(aie::eShaderStage::FRAGMENT, "../shaders/simple.frag");
	//if (m_Shader.link() == false)
	//{
	//	printf("Shader Error: %s\n", m_Shader.getLastError());
	//	return false;
	//}
	//if (m_GridTexture.load("../data/numbered_grid.tga") == false)
	//{
	//	printf("Failed to load texture!\n");
	//	return false;
	//}

	//Mesh::Vertex cubeVertices[8];
	//cubeVertices[0].position = { -0.5f	, 0	, 0.5f	, 1 };		// Down Bottom-Right
	//cubeVertices[1].position = { 0.5f	, 0	, 0.5f	, 1 };		// Down Top-Right
	//cubeVertices[2].position = { -0.5f	, 0	, -0.5f	, 1 };		// Down Bottom-Left
	//cubeVertices[3].position = { 0.5f	, 0	, -0.5f	, 1 };		// Down Top-Left
	//cubeVertices[4].position = { -0.5f	, 1.0f	, 0.5f	, 1 };	// Up Bottom-Right
	//cubeVertices[5].position = { 0.5f	, 1.0f	, 0.5f	, 1 };	// Up Top-Right
	//cubeVertices[6].position = { -0.5f	, 1.0f	, -0.5f	, 1 };	// Up Bottom-Left
	//cubeVertices[7].position = { 0.5f	, 1.0f	, -0.5f	, 1 };	// Up Top-Left

	//unsigned int cubeIndices[36] = { 0, 1, 2, 2, 1, 3, 4, 5, 6, 6, 7, 5, 0, 2, 4, 4, 6, 2, 1, 3, 5, 5, 7, 3, 0, 1, 4, 5, 4, 1 , 2, 3, 6, 7, 6, 3 };
	//m_CubeTransform = mat4(1.0);	//m_CubeTransform = glm::translate(m_CubeTransform, vec3(0, 0, -4));	//m_CubeTransform = glm::scale(m_CubeTransform, vec3(3, 1, 3));	//m_CubeMesh.initialise(8, cubeVertices, 36, cubeIndices);

	//m_CubeMesh.initialiseQuad();

	// Rendering Models
	m_ModelShader.loadShader(aie::eShaderStage::VERTEX, "../shaders/textured.vert");
	m_ModelShader.loadShader(aie::eShaderStage::FRAGMENT, "../shaders/textured.frag");
	if (m_ModelShader.link() == false)
	{
		printf("Shader Error: %s\n", m_ModelShader.getLastError());
		return false;
	}
	if (m_ModelMesh.load("../data/soulspear/soulspear.obj") == false)
	{
		printf("Model Mesh Error!\n");
		return false;
	}

	m_ModelTransform = mat4(1.0);
	//m_ModelTransform = glm::translate(m_ModelTransform, vec3(0, 1, -4));
	//m_ModelTransform = glm::scale(m_ModelTransform, vec3(0.5f, 0.5f, 0.5f));

	return true;
}

void ComputerGraphics::shutDown()
{
	Gizmos::destroy();
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

bool ComputerGraphics::update(float deltaTime)
{
	if (glfwWindowShouldClose(m_Window) == true || glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	m_Camera->update(deltaTime);

	return true;
}

void ComputerGraphics::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::clear();

	// DRAW BEGIN

	Gizmos::addTransform(mat4(1));

	vec4 white(1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), white);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), white);
	}

	Gizmos::addSphere(vec3(0, 0, 0), 1, 10, 10, vec4(0.97f, 0.71f, 0.22f, 0.5f));

	mat4 pvm = m_Camera->getProjectionView() * m_CubeTransform;

	//m_Shader.bind();
	//m_Shader.bindUniform("ProjectionViewModel", pvm);

	//m_Shader.bindUniform("diffuseTexture", 0);
	//m_GridTexture.bind(0);

	//m_CubeMesh.draw();

	m_ModelShader.bind();
	pvm = m_Camera->getProjectionView() * m_ModelTransform;
	m_ModelShader.bindUniform("ProjectionViewModel", pvm);
	m_ModelMesh.draw();

	// DRAW END

	Gizmos::draw(m_Camera->getProjectionView());	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}
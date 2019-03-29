#include "ComputerGraphics.h"
#include "FreeCamera.h"

#include <iostream>

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
	m_Camera->setLookAt(vec3(10, 5, 0), vec3(0), vec3(0, 1, 0));

	m_Light1.direction = vec3(1, 0, 0);
	m_Light1.diffuse = { 1, 1, 0 };
	m_Light1.specular = { 1, 1, 0 };

	m_Light2.direction = vec3(0, -1, 0);
	m_Light2.diffuse = { 1, 1, 0 };
	m_Light2.specular = { 1, 1, 0 };

	m_AmbientLight = { 0.25f, 0.25f, 0.25f };

	/// Rendering Geometry
	//m_GeoShader.loadShader(aie::eShaderStage::VERTEX, "../shaders/phong.vert");
	//m_GeoShader.loadShader(aie::eShaderStage::FRAGMENT, "../shaders/phong.frag");
	//if (m_GeoShader.link() == false)
	//{
	//	printf("Shader Error: %s\n", m_GeoShader.getLastError());
	//	return false;
	//}
	//if (m_GridTexture.load("../data/numbered_grid.tga") == false)
	//{
	//	printf("Failed to load texture!\n");
	//	return false;
	//}

	//m_GeoTransform = {
	//10,0,0,0,
	//0,10,0,0,
	//0,0,10,0,
	//0,0,0,1
	//};
	//m_GeoMesh.initialiseQuad();

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

	/// Rendering Models
	m_ModelShader.loadShader(aie::eShaderStage::VERTEX, "../shaders/phong.vert");
	m_ModelShader.loadShader(aie::eShaderStage::FRAGMENT, "../shaders/phong.frag");
	if (m_ModelShader.link() == false)
	{
		printf("Shader Error: %s\n", m_ModelShader.getLastError());
		return false;
	}
	if (m_ModelMesh.load("../data/soulspear/soulspear.obj", true, true) == false)
	{
		printf("Model Mesh Error!\n");
		return false;
	}

	m_ModelTransform = mat4(1.0);
	m_ModelTransform = glm::translate(m_ModelTransform, vec3(0, 1, 0));
	m_ModelTransform = glm::scale(m_ModelTransform, vec3(0.5f, 0.5f, 0.5f));

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

	m_ModelTransform = glm::rotate(m_ModelTransform, glm::radians(10.0f) * deltaTime, vec3(0, 0.5f, 0));

	m_Light1.direction = vec3(cos(glfwGetTime()), 0, sin(glfwGetTime()));
	m_Camera->update(deltaTime);

	return true;
}

void ComputerGraphics::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::clear();

	/// DRAW BEGIN

	Gizmos::addTransform(mat4(1));

	vec4 white(1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), white);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), white);
	}

	Gizmos::addSphere(vec3(0, 0, 0), 1, 10, 10, vec4(0.97f, 0.71f, 0.22f, 0.25f));

	mat4 pvm;

	/// Geometry
	//m_GeoShader.bind();

	//pvm = m_Camera->getProjectionView() * m_GeoTransform;

	//m_GeoShader.bindUniform("Ia", m_AmbientLight);
	//m_GeoShader.bindUniform("Id", m_Light.diffuse);
	//m_GeoShader.bindUniform("Is", m_Light.specular);
	//m_GeoShader.bindUniform("LightDirection", m_Light.direction);

	//m_GeoShader.bindUniform("CameraPosition", vec3(glm::inverse(m_Camera->getView())[3]));
	//m_GeoShader.bindUniform("ProjectionViewModel", pvm);
	//m_GeoShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_GeoTransform)));
	////m_GeoShader.bindUniform("diffuseTexture", 0);	For binding a texture to the GeoShader

	////m_GridTexture.bind(0);	For texture binding

	//m_GeoMesh.draw();

	/// Model
	m_ModelShader.bind();

	pvm = m_Camera->getProjectionView() * m_ModelTransform;

	m_ModelShader.bindUniform("Ia", m_AmbientLight);

	m_ModelShader.bindUniform("Id1", m_Light1.diffuse);
	m_ModelShader.bindUniform("Is1", m_Light1.specular);
	m_ModelShader.bindUniform("LightDirection1", m_Light1.direction);

	m_ModelShader.bindUniform("Id2", m_Light2.diffuse);
	m_ModelShader.bindUniform("Is2", m_Light2.specular);
	m_ModelShader.bindUniform("LightDirection2", m_Light2.direction);

	m_ModelShader.bindUniform("ProjectionViewModel", pvm);
	m_ModelShader.bindUniform("CameraPosition", vec3(glm::inverse(m_Camera->getView())[3]));
	m_ModelShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_ModelTransform)));
	m_ModelShader.bindUniform("DiffuseTexture", 0);

	m_ModelMesh.draw();

	/// DRAW END

	Gizmos::draw(m_Camera->getProjectionView());	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}
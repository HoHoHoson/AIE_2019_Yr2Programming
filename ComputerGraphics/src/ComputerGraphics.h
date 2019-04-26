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

/*
	my OpenGL computer graphics app
*/
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

	// Pointers to GLFWindow and Camera
	GLFWwindow* m_Window;
	Camera* m_Camera;

	aie::Texture m_GridTexture;

	// ShaderProgram, Mesh and transform variables for a mesh
	aie::ShaderProgram	m_GeoShader;
	Mesh				m_GeoMesh;
	mat4				m_GeoTransform;

	// ShaderProgram, Mesh and transform variables for a model
	aie::ShaderProgram	m_ModelShader;
	aie::OBJMesh		m_ModelMesh;
	mat4				m_ModelTransform;

	/*
		Struct that stores the direction, diffuse and specular of a directional light.
		Needs to be passed into shaders
	*/
	struct Light
	{
		vec3 direction;
		vec3 diffuse;
		vec3 specular;
	};
	Light				m_Light1;
	Light				m_Light2;

	// Vector3 that stores the value of the scenes ambient light
	vec3				m_AmbientLight;
};
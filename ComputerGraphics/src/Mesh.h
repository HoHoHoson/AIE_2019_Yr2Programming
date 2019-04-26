#pragma once
#include <glm/ext.hpp>

using glm::vec2;
using glm::vec4;

/*
	Class that can create and draw meshes by utilising OpenGL
*/
class Mesh
{
public:

	Mesh() : m_TriCount(0), m_VAO(0), m_VBO(0), m_IBO(0) {}
	virtual ~Mesh();

	virtual void draw();

	/* Struct that stores the position, normal and texture coordinate of a Vertex */
	struct Vertex
	{
		vec4 position;
		vec4 normal;
		vec2 texCoord;
	};

	/*
		@brief Initialises a mesh with specified vertices
		@param Number of vertexes in the mesh
		@param Array of Vertex structs that stores the position, normal and texCoord of a vertex
		@param Number of indices
		@param Array of unsigned int that dictates which vertices form a triangle, each int is a index of the Vertex array, every 3 ints form one triangle of the mesh from the specified vertices
	*/
	void initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);

	/* Initialises a preset, 2D 1x1 square plane centered at the origin, as the mesh */
	void initialiseQuad();

protected:

	unsigned int m_TriCount;
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
};
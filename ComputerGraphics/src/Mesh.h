#pragma once
#include <glm/ext.hpp>

using glm::vec2;
using glm::vec4;


class Mesh
{
public:

	Mesh() : m_TriCount(0), m_VAO(0), m_VBO(0), m_IBO(0) {}
	virtual ~Mesh();

	virtual void draw();

	struct Vertex
	{
		vec4 position;
		vec4 normal;
		vec2 texCoord;
	};

	void initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);
	void initialiseQuad();

protected:

	unsigned int m_TriCount;
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
};
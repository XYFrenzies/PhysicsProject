#pragma once
#include <glm/glm.hpp>
class Mesh
{
public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {};
	virtual ~Mesh();

	struct Vertex 
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 textCoord;
	};
	void InitialiseQuad();
	void Initialise(unsigned int a_vertexCount, 
		const Vertex* a_vertices, unsigned int a_indexCount = 0,
		unsigned int* a_indices = nullptr);

	virtual void Draw();

private:

protected:
	unsigned int	triCount;
	unsigned int	vao, //Vertex Array Object
					vbo, //Vectex Buffer Object
					ibo; //Index Buffer Object
};


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
	virtual void Draw();

private:

protected:
	unsigned int	triCount;
	unsigned int	vao, //Vertex Array Object
					vbo, //Vectex Buffer Object
					ibo; //Index Buffer Object
};


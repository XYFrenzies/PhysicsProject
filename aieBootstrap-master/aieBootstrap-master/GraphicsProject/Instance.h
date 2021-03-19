#pragma once
#include <glm/glm.hpp>

class Camera;
class Scene;

namespace aie 
{
	class OBJMesh;
	class ShaderProgram;
}

class Instance
{
public:
	Instance(glm::mat4 a_transform, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader);
	Instance(glm::vec3 a_position, glm::vec3 a_eularAngles,
		glm::vec3 a_scale, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader);
	void Draw(Scene* a_scene);
	static glm::mat4 MakeTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale);
	void Update(float m_deltaTime);
	glm::vec3& GetPosition() { return m_pos; }
	glm::vec3& GetRotation() { return m_rot; }
	glm::vec3& GetScale() { return m_scale; }

protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;
	glm::vec3 m_pos = glm::vec3(0);
	glm::vec3 m_rot = glm::vec3(0);
	glm::vec3 m_scale = glm::vec3(1);
};


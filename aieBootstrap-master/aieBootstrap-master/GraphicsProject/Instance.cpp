#include "Instance.h"
#include <Application.h>
#include <Texture.h>
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Shader.h"
#include <glm/ext.hpp>

Instance::Instance(glm::mat4 a_transform, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader)
	: m_transform(a_transform), m_mesh(a_mesh), m_shader(a_shader)
{
}

Instance::Instance(glm::vec3 a_position, glm::vec3 a_eularAngles, glm::vec3 a_scale, 
	aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader)
	: m_mesh(a_mesh), m_shader(a_shader)
{
	m_transform = MakeTransform(a_position, a_eularAngles, a_scale);
}

void Instance::Draw(Scene* a_scene)
{
	m_shader->bind();
	auto pvm = a_scene->GetCamera()->GetProjectionMatrix(a_scene->GetWindowSize().x, a_scene->GetWindowSize().y)
		* a_scene->GetCamera()->GetViewMatrix() * m_transform;
	//Bind the transform
	m_shader->bindUniform("ProjectionViewModel", pvm);
	m_shader->bindUniform("CameraPosition", a_scene->GetCamera()->GetPosition());//m_multipleCameras[camValue].GetPosition() FInd this equivilent
	m_shader->bindUniform("AmbientColour", a_scene->GetAmbientLight());
	m_shader->bindUniform("LightColor", a_scene->GetLight().m_color);
	m_shader->bindUniform("LightDirection", a_scene->GetLight().m_direction);

	//Bind the texture to a location of yout choice (0).
	m_shader->bindUniform("ModelMatrix", m_transform);
	//Draw the nesh
	m_mesh->draw();
}

glm::mat4 Instance::MakeTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale)
{
	return glm::translate(glm::mat4(1), a_position) * glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.x), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), a_scale);
}

#pragma once
#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Planet.h"
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void Planets();
	bool LoadShaderAndMesh();
	void DrawShadersAndMeshes(glm::mat4, glm::mat4);

protected:

	//====SHADER====
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_bunnyShader;
	//===================
	//Basic Plane
	Mesh				m_quadMesh;
	glm::mat4			m_quadTransform;

	//Create a bunny with a flat colour
	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
	// planets
	Planet* m_planet;
	std::vector<Planet*> m_planetsArray;
};
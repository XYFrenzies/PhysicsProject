#pragma once
#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Planet.h"
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Camera.h"
#include "Scene.h"
#include "ParticleEmmiter.h"
class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	bool LoadShaderAndMesh(Light a_light);
	void IMGUI_Logic();
	void IMGUI_Transform();
	void IMGUI_AddCamera();
protected:
	//Scene
	Scene* m_scene;
	//Camera and the amount that is available
	std::vector<Camera> m_multipleCameras;
	glm::mat4 cameraTransform;
	float numOfCamerasInScene = 4;
	int camValue = 0;
	//====SHADER====
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_normalMapShader;
	aie::ShaderProgram m_particleShader;
	//===================
	//Create a bunny with transforms and positions
	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;
	// Camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
	//Create a SoulSpear
	aie::OBJMesh		m_spearMesh;
	glm::mat4			m_spearTransform;
	//Create a Lightsaber
	aie::OBJMesh		m_sideWinderMesh;
	glm::mat4			m_sideWinderTransform;
	glm::vec3 m_rotationSpeed = glm::vec3(0,0,0);
	float m_rotationDistance = 1;
	glm::vec3 newDirection = glm::vec3(0);
	glm::vec3 ambientLight = glm::vec3(0);
	glm::vec3 offsetObject = glm::vec3(0);

	//Particle emitter
	ParticleEmmiter* m_emitter;
	glm::mat4 m_particleTransform;
};
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
class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void Planets();

	bool LoadShaderAndMesh(Light a_light);
	void IMGUI_Logic();
	void IMGUI_Transform();
	void IMGUI_AddCamera();
	void AddCamera();
protected:
	//Camera
	std::vector<Camera> m_multipleCameras;
	float numOfCamerasInScene = 2;
	float camValue = 0;
	//====SHADER====
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_textureShader;
	aie::ShaderProgram m_normalMapShader;
	//===================
	//Texture
	aie::Texture		m_gridTexture;
	//Basic Plane
	Mesh				m_quadMesh;
	glm::mat4			m_quadTransform = glm::mat4(0);
	// Camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
	// planets
	Planet* m_planet;
	std::vector<Planet*> m_planetsArray;
	//Create a soulspear
	aie::OBJMesh		m_spearMesh;
	glm::mat4			m_spearTransform;
	//Creae a lightsaber
	aie::OBJMesh		m_lightSaberMesh;
	glm::mat4			m_lightSaberTransform;

	std::vector<Light> m_lights;
	int numLights = 2;

	Scene* m_scene;
};
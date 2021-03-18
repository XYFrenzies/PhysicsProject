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
protected:
	//Scene
	Scene* m_scene;
	//Camera and the amount that is available
	std::vector<Camera> m_multipleCameras;
	float numOfCamerasInScene = 2;
	int camValue = 0;
	//====SHADER====
	aie::ShaderProgram m_normalMapShader;
	//===================
	// Camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
	// planets
	Planet* m_planet;
	std::vector<Planet*> m_planetsArray;
	//Create a SoulSpear
	aie::OBJMesh		m_spearMesh;
	glm::mat4			m_spearTransform;
	//Create a Lightsaber
	aie::OBJMesh		m_lightSaberMesh;
	glm::mat4			m_lightSaberTransform;

};
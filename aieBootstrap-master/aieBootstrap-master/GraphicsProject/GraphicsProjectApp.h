#pragma once
#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Planet.h"
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Camera.h"
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
	void IMGUI_Logic();
	void IMGUI_Transform();
	void IMGUI_AddCamera();
	void AddCamera();
protected:
	//Camera
	Camera			   m_camera;
	std::vector<Camera> m_multipleCameras;
	float numOfCamerasInScene = 2;
	float camValue = 0;
	//====SHADER====
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_bunnyShader;
	aie::ShaderProgram m_dragonShader;
	aie::ShaderProgram m_buddhaShader;
	aie::ShaderProgram m_lucyShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_textureShader;
	aie::ShaderProgram m_normalMapShader;
	//===================
	//Texture
	aie::Texture		m_gridTexture;
	//Basic Plane
	Mesh				m_quadMesh;
	glm::mat4			m_quadTransform = glm::mat4(0);
	//Create a bunny with a flat colour
	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTranslation;
	glm::mat4			m_bunnyTransform;
	glm::mat4			m_bunnyRot;
	glm::mat4			m_bunnyScale;

	//Creates the dragon
	aie::OBJMesh		m_dragonMesh;
	glm::mat4			m_dragonTransform;
	glm::mat4			m_dragonTranslation;
	glm::mat4			m_dragonRot;
	glm::mat4			m_dragonScale;
	//Creates the Buddha
	aie::OBJMesh		m_buddhaMesh;
	glm::mat4			m_buddhaTransform;
	glm::mat4			m_buddhaTranslation;
	glm::mat4			m_buddhaRot;
	glm::mat4			m_buddhaScale;
	//Creates a statue called lucy
	aie::OBJMesh		m_lucyMesh;
	glm::mat4			m_lucyTransform;
	glm::mat4			m_lucyTranslation;
	glm::mat4			m_lucyRot;
	glm::mat4			m_lucyScale;
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

	struct Light 
	{
		glm::vec3 direction;
		glm::vec3 colour;
	};

	Light				m_light;
	glm::vec3			m_ambientLight;
};
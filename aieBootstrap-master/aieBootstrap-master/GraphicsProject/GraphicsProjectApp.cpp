#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <ImGui.h>
using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsProjectApp::GraphicsProjectApp(): m_ambientLight(),
 m_planet(), m_projectionMatrix(), m_viewMatrix() 
{
	for (int i = 0; i < numOfCamerasInScene; i++)
	{
		Camera cam;
		m_multipleCameras.push_back(cam);
	}
	for (int i = 0; i < numLights; i++)
	{
		Light m_light;
		m_lights.push_back(m_light);
	}
}

GraphicsProjectApp::~GraphicsProjectApp() {

}

bool GraphicsProjectApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);
	Planets();
	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 
		(float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	for (int i = 0; i < m_lights.size() - 1; i++)
	{
		lightColor[i] = m_lights[i].colour;
	}
	for (int i = 0; i < m_lights.size() - 1; i++)
	{
		lightColor[i] = { 1, 1, 1 };
	}

	m_ambientLight = { 1, 1, 1 };
	return LoadShaderAndMesh();
}

void GraphicsProjectApp::shutdown() {
	Gizmos::destroy();
	for (auto planet : m_planetsArray)
		delete planet;
}

void GraphicsProjectApp::update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	//Updating and Drawing the position of the planets per frame.
	for (auto planet : m_planetsArray)
	{
		planet->MakeGizmo();
		if(planet->GetPosition() != glm::vec3(0, 0.5f, 0))
			planet->Update(deltaTime);
	}
	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));
	aie::Input* input = aie::Input::getInstance();


	if (input->wasKeyPressed(aie::INPUT_KEY_UP) && camValue < m_multipleCameras.size() - 1)
		camValue++;
	else if(input->wasKeyPressed(aie::INPUT_KEY_UP) && camValue >= m_multipleCameras.size() - 1)
		camValue = 0;

	std::vector<glm::vec3> lightDir;
	std::vector<glm::vec3> lightColor;
	for (int i = 0; i < m_lights.size() - 1; i++)
	{
		lightDir[i] = m_lights[i].direction;
	}

	m_multipleCameras[camValue].Update(deltaTime);
	IMGUI_Logic();
	IMGUI_Transform();
	IMGUI_AddCamera();
	//Rotates the light direction per time variable.
	float time = getTime();
	for (int i = 0; i < m_lights.size() - 1; i++)
	{
		lightDir[i] = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));
	}
	// quit if we press escape
	aie::Input* newinput = aie::Input::getInstance();

	if (newinput->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();
	glm::mat4 projectionMatrix = m_multipleCameras[camValue].GetProjectionMatrix(
		(float)getWindowWidth(), (float)getWindowHeight());
	glm::mat4 viewMatrix = m_multipleCameras[camValue].GetViewMatrix();
	// update perspective based on screen size
	DrawShadersAndMeshes(projectionMatrix, viewMatrix);

	Gizmos::draw(projectionMatrix * viewMatrix);
}

void GraphicsProjectApp::Planets()
{
	Planet* sun = new Planet({ 0, 0.5f, 0 }, 0, 1, 0, glm::vec4(1, 1, 0, 1));
	Planet* mercury = new Planet({ 1, 0.5f, 0 }, 3.0f, 0.2f, 10, glm::vec4(1,0,0,1));
	m_planetsArray.push_back(sun);
	m_planetsArray.push_back(mercury);
}

void GraphicsProjectApp::AddCamera()
{
}

bool GraphicsProjectApp::LoadShaderAndMesh()
{
#pragma region Load Shaders
#pragma region TextureShader
	m_textureShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_textureShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");
	if (!m_textureShader.link())
	{
		printf("Texture Shader had an error: %s\n", m_textureShader.getLastError());
		return false;
	}


#pragma endregion
#pragma region NormalMapShader
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");
	if (m_normalMapShader.link() == false)
	{
		printf("Normal Map shader had an error %s\n", m_normalMapShader.getLastError());
		return false;
	}
#pragma endregion

#pragma endregion

#pragma region Mesh Logic
#pragma region SoulSpear
	if (!m_spearMesh.load("./soulspear/soulspear.obj", true, true))
	{
		printf("Soulspear Mesh has had an error.");
		return false;
	}

	m_spearTransform = {
		1.0f, 0, 0, 0,
		0, 1.0f, 0, 0,
		0, 0, 1.0f, 0,
		0, 0, 0, 1
	};
#pragma endregion
#pragma region LightSaber
	if (!m_lightSaberMesh.load("./lightsaber/KKls.obj", true, true))
	{
		printf("LightSaber Mesh has had an error.");
		return false;
	}

	m_lightSaberTransform = {
		0.1f, 0, 0, 0,
		0, 0.1f, 0, 0,
		0, 0, 0.1f, 0,
		5, 0, 5, 1
	};
#pragma endregion

#pragma endregion

#pragma region Phong
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");
	if (m_phongShader.link() == false)
	{
		printf("Phong shader had an error %s\n", m_phongShader.getLastError());
		return false;
	}
#pragma endregion

	return true;
}

void GraphicsProjectApp::DrawShadersAndMeshes(glm::mat4 a_projectionMatrix, 
	glm::mat4 a_viewMatrix)
{
	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);
#pragma region SoulSpear
	m_normalMapShader.bind();

	//Bind the transform
	pvm = a_projectionMatrix * a_viewMatrix * m_spearTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("CameraPosition", m_multipleCameras[camValue].GetPosition());
	m_normalMapShader.bindUniform("AmbientColour", m_ambientLight);
	m_normalMapShader.bindUniform("lightAmount", (int)m_lights.size());
	m_normalMapShader.bindUniform("lightArray", (int)m_lights.data());

	//Bind the texture to a location of yout choice (0).
	m_normalMapShader.bindUniform("ModelMatrix", m_spearTransform);
	//Draw the nesh
	m_spearMesh.draw();
	
#pragma endregion
#pragma region LightSaber
	m_normalMapShader.bind();

	//Bind the transform
	pvm = a_projectionMatrix * a_viewMatrix * m_lightSaberTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("CameraPosition", m_multipleCameras[camValue].GetPosition());
	m_normalMapShader.bindUniform("AmbientColour", m_ambientLight);
	m_normalMapShader.bindUniform("lightAmount", (int)m_lights.size());
	m_normalMapShader.bindUniform("lightArray", (int)m_lights.data());
	//Bind the texture to a location of yout choice (0).
	m_normalMapShader.bindUniform("ModelMatrix", m_lightSaberTransform);
	//Draw the nesh
	m_lightSaberMesh.draw();

#pragma endregion

}

void GraphicsProjectApp::IMGUI_Logic()
{
	ImGui::Begin("Scene Light Settings");
	//ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f, -1.0f);
	//ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();
}

void GraphicsProjectApp::IMGUI_Transform()
{
	ImGui::Begin("Object Transform");
	//ImGui::DragFloat3("Bunny Movement", &m_bunnyTranslation[3][0], 0.1f, -15.0f, 15.0f);
	//ImGui::DragFloat3("Dragon Movement", &m_dragonTranslation[3][0], 0.1f, -15.0f, 15.0f);
	//ImGui::DragFloat3("Buddha Movement", &m_buddhaTranslation[3][0], 0.1f, -15.0f, 15.0f);
	//ImGui::DragFloat3("Lucy Movement", &m_lucyTranslation[3][0], 0.1f, -15.0f, 15.0f);
	ImGui::End();
}

void GraphicsProjectApp::IMGUI_AddCamera()
{
	ImGui::Begin("Camera Control");

	if (ImGui::Button("Add Camera"))
	{
		Camera cam;
		m_multipleCameras.push_back(cam);
	}
	for (int i = 0; i < m_multipleCameras.size(); i++)
	{
		
		if (ImGui::Button("New Camera" + i))
		{
			camValue = i;
		}

	}
	ImGui::End();
}

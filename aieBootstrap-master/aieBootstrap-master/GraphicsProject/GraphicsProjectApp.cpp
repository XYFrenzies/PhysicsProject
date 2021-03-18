#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <ImGui.h>
#include "Scene.h"
#include "Instance.h"
#include <string>
using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsProjectApp::GraphicsProjectApp() :
m_planet(), m_projectionMatrix(), m_viewMatrix(), m_spearTransform(),
m_scene(), m_lightSaberTransform()
{
	for (int i = 0; i < numOfCamerasInScene; i++)
	{
		Camera cam;
		m_multipleCameras.push_back(cam);
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
	Light light;
	return LoadShaderAndMesh(light);
}

void GraphicsProjectApp::shutdown() {
	Gizmos::destroy();
	for (auto planet : m_planetsArray)
		delete planet;
	delete m_scene;
}

void GraphicsProjectApp::update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	//Updating and Drawing the position of the planets per frame.
	for (auto planet : m_planetsArray)
	{
		planet->MakeGizmo();
		if (planet->GetPosition() != glm::vec3(0, 0.5f, 0))
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

	// If there are more cameras to rotate to.
	if (input->wasKeyPressed(aie::INPUT_KEY_UP) && camValue < m_multipleCameras.size() - 1)
		camValue++;
	//If the camera values are beyond the amount within the scene.
	else if (input->wasKeyPressed(aie::INPUT_KEY_UP) && camValue >= m_multipleCameras.size() - 1)
		camValue = 0;

	m_multipleCameras[(unsigned int)camValue].Update(deltaTime);
	IMGUI_Logic();
	IMGUI_Transform();
	IMGUI_AddCamera();
	//Rotates the light direction per time variable.
	float time = getTime();

	for (int i = 0; i < m_scene->GetNumLights(); i++)
	{
		m_scene->GetPointLights()[i].m_direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));
	}
	
	for (auto instance : m_scene->m_instances)
	{
		instance->Update(deltaTime);
	}
	// quit if we press escape
	aie::Input* newinput = aie::Input::getInstance();

	if (newinput->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();
	glm::mat4 projectionMatrix = m_multipleCameras[(unsigned int)camValue].GetProjectionMatrix(
		(float)getWindowWidth(), (float)getWindowHeight());
	glm::mat4 viewMatrix = m_multipleCameras[(unsigned int)camValue].GetViewMatrix();
	//Detemines if the current camera is the camera thats being used.
	if (m_scene->GetCamera() != &m_multipleCameras[(unsigned int)camValue])
	{
		//If it isnt, we set the new camera.
		m_scene->SetCamera(&m_multipleCameras[(unsigned int)camValue]);
	}
	m_scene->Draw();
	// update perspective based on screen size
	Gizmos::draw(projectionMatrix * viewMatrix);
}

void GraphicsProjectApp::Planets()
{
	//Creates two instances of planets within the scene.
	Planet* sun = new Planet({ 0, 0.5f, 0 }, 0, 1, 0, glm::vec4(1, 1, 0, 1));
	Planet* mercury = new Planet({ 1, 0.5f, 0 }, 3.0f, 0.2f, 10, glm::vec4(1, 0, 0, 1));
	m_planetsArray.push_back(sun);
	m_planetsArray.push_back(mercury);
}
bool GraphicsProjectApp::LoadShaderAndMesh(Light a_light)
{
#pragma region Load Shaders
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
#pragma region LoadInstances

	//This is to initialise a scene with two objects within it.
	//These two objects have a scale and position predefined by their 
	m_scene = new Scene(&m_multipleCameras[(unsigned int)camValue], glm::vec2(getWindowWidth(), getWindowHeight()),
		a_light, glm::vec3(0.25f));

	m_scene->AddInstance(new Instance(glm::vec3(m_spearTransform[3][0],
		m_spearTransform[3][1], m_spearTransform[3][2]),
		glm::vec3(0, 30, 0), glm::vec3(m_spearTransform[0][0],
			m_spearTransform[1][1], m_spearTransform[2][2]),
		&m_spearMesh, &m_normalMapShader));
	m_scene->AddInstance(new Instance(glm::vec3(glm::vec3(m_lightSaberTransform[3][0],
		m_lightSaberTransform[3][1], m_lightSaberTransform[3][2])),
		glm::vec3(0, 30, 0), glm::vec3(m_lightSaberTransform[0][0],
			m_lightSaberTransform[1][1], m_lightSaberTransform[2][2]),
		&m_lightSaberMesh, &m_normalMapShader));

	//Adding a red light to the left of the scene.
	m_scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1,0,0), 50));
	//Adding a green light to the right of the scene
	m_scene->GetPointLights().push_back(Light(vec3(-5, -3, 0), vec3(0, 1, 0), 50));

#pragma endregion
	return true;
}
void GraphicsProjectApp::IMGUI_Logic()
{
	ImGui::Begin("Scene Light Settings");
	for (int i = 0; i < m_scene->GetNumLights(); i++)
	{
		ImGui::DragFloat3((std::string("PointLight Direction ") + std::to_string(i)).c_str(), 
			&m_scene->GetPointLights()[i].m_direction[0], 0.1f, -1.0f, -1.0f);
		ImGui::DragFloat3((std::string("PointLight Colour ") + std::to_string(i)).c_str(), 
			&m_scene->GetPointLights()[i].m_color[0], 0.1f, 0.0f, 50.0f);
	}
	ImGui::End();
}

void GraphicsProjectApp::IMGUI_Transform()
{
	ImGui::Begin("Object Transform");
	ImGui::DragFloat3("LightSaber Movement", &m_scene->GetInstance(1)->GetPosition()[0], 0.1f, -15.0f, 15.0f);
	ImGui::DragFloat3("SoulSpear Movement", &m_spearTransform[3][0], 0.1f, -15.0f, 15.0f);
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
		if (ImGui::Button((std::string("New Camera ") + std::to_string(i)).c_str()))
		{
			camValue = i;
		}
	}
	ImGui::End();
}

#define	GLM_ENABLE_EXPERIMENTAL
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

GraphicsProjectApp::GraphicsProjectApp() : m_spearTransform(),
m_scene(), m_sideWinderTransform(), m_bunnyTransform(), m_emitter(),
m_particleTransform()
{//Creates a set amount of cameras to begin with.
	Camera newCam;
	m_multipleCameras.push_back(newCam);
	Camera cam;
	cam.SetStaticCam();
	m_multipleCameras.push_back(cam);
	cam.SetPosition(glm::vec3(0, 2, -10));
	m_multipleCameras.push_back(cam);
	cam.SetPosition(glm::vec3(0, 10, 0));
	m_multipleCameras.push_back(cam);
	//Initiates the particles.
	m_particleStartColour = glm::vec4(1, 0, 0, 1);
	m_particleEndColour = glm::vec4(1, 1, 0, 1);
	m_particleStartScale = 1;
	m_particleEndScale = 0.1f;

}

GraphicsProjectApp::~GraphicsProjectApp() {

}

bool GraphicsProjectApp::startup() {

	setBackgroundColour(0.25f, 0.25f, 0.25f);
	for (int i = 0; i < m_multipleCameras.size(); i++)
	{
		m_multipleCameras[i].SetPerspective(glm::pi<float>() * 0.25f,
			(float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
		m_multipleCameras[i].SetLookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	}
	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);
	//Creates an instance of a light source.
	Light light;
	//Creating a new emmiter

	m_emitter = new ParticleEmmiter();
	m_emitter->Initalise(1000, 500, 0.1f, 1.0f, 1.0f, 
		5, m_particleStartScale, m_particleEndScale, m_particleStartColour, m_particleEndColour);
	return LoadShaderAndMesh(light);
}

void GraphicsProjectApp::shutdown() {
	Gizmos::destroy();
	delete m_scene;
	delete m_emitter;
}

void GraphicsProjectApp::update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

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
	m_scene->SetAmbientLight(ambientLight);
	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));
	aie::Input* input = aie::Input::getInstance();

	// If there are more cameras to rotate to.
	if (input->wasKeyPressed(aie::INPUT_KEY_UP) && camValue < m_multipleCameras.size() - 1)
		camValue++;
	//If the camera values are beyond the amount within the scene.
	else if (input->wasKeyPressed(aie::INPUT_KEY_UP) && camValue >= m_multipleCameras.size() - 1)
		camValue = 0;

	Camera cam = m_multipleCameras[(unsigned int)camValue];
	//Updates the specific camera that is in the scene.
	m_multipleCameras[(unsigned int)camValue].Update(deltaTime);
	//Logic of the gui
	IMGUI_Logic();
	IMGUI_Transform();
	IMGUI_AddCamera();
	IMGUI_Particle();
	//Time variable which is scaled with when the application is started and continues per frame.
	float time = getTime();

	//Creates sphereical point lights.
	Gizmos::addSphere(m_scene->GetPointLights()[0].m_direction , 0.25f, 8, 8,
		glm::vec4(m_scene->GetPointLights()[0].m_color, 1));
	Gizmos::addSphere(m_scene->GetPointLights()[1].m_direction, 0.25f, 8, 8,
		glm::vec4(m_scene->GetPointLights()[1].m_color, 1));
	Gizmos::addSphere(m_scene->GetPointLights()[2].m_direction + offsetObject, 0.25f, 8, 8,
		glm::vec4(m_scene->GetPointLights()[2].m_color, 1));
	
	//Makes the spheres rotate depending on the user input (gui)
	m_scene->GetPointLights()[2].m_direction.x = glm::cos(time * m_rotationSpeed.x) * m_rotationDistance;
	m_scene->GetPointLights()[2].m_direction.y = glm::sin(time * m_rotationSpeed.y) * m_rotationDistance;
	m_scene->GetPointLights()[2].m_direction.z = glm::sin(time * m_rotationSpeed.z) * m_rotationDistance;

	//The mat4 is the only issue im having with the emittion of the particles.
	m_emitter->Update(deltaTime, m_multipleCameras[camValue].GetPosition(), m_particleStartColour, m_particleEndColour, 
		m_particleStartScale, m_particleEndScale);
	//Updates all the instances of the objects in the scene with their position, rotation and scale.
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
	//Creates an instance of the projection matrix
	glm::mat4 projectionMatrix = m_multipleCameras[camValue].GetProjectionMatrix();
	//Creates an instance of the view matrix
	glm::mat4 viewMatrix = m_multipleCameras[(unsigned int)camValue].GetViewMatrix();
	//Detemines if the current camera is the camera thats being used.
	if (m_scene->GetCamera() != &m_multipleCameras[(unsigned int)camValue])
	{
		//If it isnt, we set the new camera.

		m_scene->SetCamera(&m_multipleCameras[(unsigned int)camValue]);
	}
	m_scene->Draw();
	// bind particle shader
	m_particleShader.bind();

	// bind particle transform
	auto pvm = projectionMatrix * viewMatrix  * m_particleTransform;
	m_particleShader.bindUniform("ProjectionViewModel", pvm);

	m_emitter->Draw();
	// update perspective based on screen size
	Gizmos::draw(projectionMatrix * viewMatrix);
}
bool GraphicsProjectApp::LoadShaderAndMesh(Light a_light)
{
#pragma region Load Shaders
#pragma region PhongShader
	//Loads the vertex and fragment shader and if the link between the two has an error.
//(Or an error has occured, it will return the error message and exits the program.)
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");
	if (!m_phongShader.link())
	{
		printf("Phong shader had an error %s\n", m_phongShader.getLastError());
		return false;
	}
#pragma endregion

#pragma region NormalMapShader
	//Loads the vertex and fragment shader and if the link between the two has an error.
//(Or an error has occured, it will return the error message and exits the program.)
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");
	if (m_normalMapShader.link() == false)
	{
		printf("Normal Map shader had an error %s\n", m_normalMapShader.getLastError());
		return false;
	}
#pragma endregion
#pragma region ParticleShader
	//Loads the vertex and fragment shader and if the link between the two has an error.
	//(Or an error has occured, it will return the error message and exits the program.)
	m_particleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/vertex.vert");
	m_particleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/vertex.frag");
	if (m_particleShader.link() == false)
	{
		printf("Particle shader had an error %s\n", m_particleShader.getLastError());
		return false;
	}
#pragma endregion

#pragma endregion

#pragma region Mesh Logic
#pragma region SoulSpear
	//Loads the SoulSPear obj, if it fails, exit the program.
	if (!m_spearMesh.load("./soulspear/soulspear.obj", true, true))
	{
		printf("Soulspear Mesh has had an error.");
		return false;
	}
	//Used to determine the scale and size.
	m_spearTransform = {
		1.0f, 0, 0, 0,
		0, 1.0f, 0, 0,
		0, 0, 1.0f, 0,
		-5, 1, 5, 1
	};
#pragma endregion
#pragma region SideWinder
	//Loads the SideWinder obj, if it fails, exit the program.
	if (!m_sideWinderMesh.load("./sidewinder/Sidewinder_tris.obj", true, true))
	{
		printf("SideWinder Mesh has had an error.");
		return false;
	}
	//Used to determine the scale and size.
	m_sideWinderTransform = {
		0.01f, 0, 0, 0,
		0, 0.01f, 0, 0,
		0, 0, 0.01f, 0,
		5, 1, 5, 1
	};
#pragma endregion
#pragma region BunnyLogic
	//Loads the bunny obj, if it fails, exit the program.
	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny Mesh Failed!!!\n");
		return false;
	}
	//Used to determine the scale and size.
	m_bunnyTransform =
	{
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		5, 0, -5, 1
	};
#pragma endregion
#pragma region Particles
	//This is used to predefine the particle to draw it into world space but to also change 
	//the position of the particles over time.
	m_particleTransform =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
#pragma endregion

#pragma endregion
#pragma region LoadInstances

	//This is to initialise a scene with two objects within it.
	//These two objects have a scale and position predefined by their 
	m_scene = new Scene(&m_multipleCameras[(unsigned int)camValue], glm::vec2(getWindowWidth(), getWindowHeight()),
		a_light, glm::vec3(0.25f));
	//Creates an instance of the soulspear with a set position, rotation and scale.
	m_scene->AddInstance(new Instance(glm::vec3(m_spearTransform[3][0],
		m_spearTransform[3][1], m_spearTransform[3][2]),
		glm::vec3(0, 30, 0), glm::vec3(m_spearTransform[0][0],
			m_spearTransform[1][1], m_spearTransform[2][2]),
		&m_spearMesh, &m_normalMapShader));
	//Creates an instance of the lightSaber with a set position, rotation and scale.
	m_scene->AddInstance(new Instance(glm::vec3(glm::vec3(m_sideWinderTransform[3][0],
		m_sideWinderTransform[3][1], m_sideWinderTransform[3][2])),
		glm::vec3(0, 30, 0), glm::vec3(m_sideWinderTransform[0][0],
			m_sideWinderTransform[1][1], m_sideWinderTransform[2][2]),
		&m_sideWinderMesh, &m_normalMapShader));
	//Creates an instance of the bunny with a set position, rotation and scale.
	m_scene->AddInstance(new Instance(glm::vec3(glm::vec3(m_bunnyTransform[3][0],
		m_bunnyTransform[3][1], m_bunnyTransform[3][2])),
		glm::vec3(0, 30, 0), glm::vec3(m_bunnyTransform[0][0],
			m_bunnyTransform[1][1], m_bunnyTransform[2][2]),
		&m_bunnyMesh, &m_phongShader));

	//Adding a red light to the left of the scene.
	m_scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));
	//Adding a green light to the right of the scene
	m_scene->GetPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));
	//Adding a blue light to the middle of the scene
	m_scene->GetPointLights().push_back(Light(vec3(5, 3, 5), vec3(0, 0, 1), 50));
#pragma endregion
	return true;
}
/// <summary>
/// The IMGUI functions are created in the form of allowing the player to change the attributes of the object in scene. 
/// </summary>
/// This gui is determining the amount of lights in the scene, determining their colour and direction. 
/// This is as well as the sunliught colour, rotation and the ambient.
void GraphicsProjectApp::IMGUI_Logic()
{
	ImGui::Begin("Scene Light Settings");
	for (int i = 0; i < m_scene->GetNumLights() - 1; i++)
	{
		ImGui::DragFloat3((std::string("PointLight Direction ") + std::to_string(i)).c_str(),
			&m_scene->GetPointLights()[i].m_direction[0], 0.1f, -1.0f, -1.0f);
		ImGui::DragFloat3((std::string("PointLight Colour ") + std::to_string(i)).c_str(),
			&m_scene->GetPointLights()[i].m_color[0], 0.1f, 0.0f, 50.0f);
	}
	ImGui::DragFloat("Sunlight RotationDistance 2", &m_rotationDistance, 0.1f, 0.0f, 50.0f);
	ImGui::DragFloat3("Sunlight Direction 2", &offsetObject[0], 0.1f, -50.0f, 50.0f);
	ImGui::DragFloat3("Sunlight RotationSpeed 2", &m_rotationSpeed[0], 0.1f, 0.0f, 50.0f);
	ImGui::DragFloat3("PointLight Colour 2", &m_scene->GetPointLights()[2].m_color[0], 0.1f, 0.0f, 50.0f);
	ImGui::DragFloat3("Ambient Light", &ambientLight[0], 0.1f, 0.0f, 50.0f);
	ImGui::End();
}
/// The gui transform is changing the soulspear, bunny and sideWinder's position, rotation and scale.
void GraphicsProjectApp::IMGUI_Transform()
{
	ImGui::Begin("Object Transform");
	ImGui::DragFloat3("SoulSpear Movement", &m_scene->GetInstance(0)->GetPosition()[0], 0.1f, -50.0f, 50.0f);
	ImGui::DragFloat3("SoulSpear Rotation", &m_scene->GetInstance(0)->GetRotation()[0], 0.1f, -180.0f, 180.0f);
	ImGui::DragFloat3("SoulSpear Scale", &m_scene->GetInstance(0)->GetScale()[0], 0.1f, 0.01f, 5.0f);
	ImGui::DragFloat3("SideWinder Movement", &m_scene->GetInstance(1)->GetPosition()[0], 0.1f, -50.0f, 50.0f);
	ImGui::DragFloat3("SideWinder Rotation", &m_scene->GetInstance(1)->GetRotation()[0], 0.1f, -180.0f, 180.0f);
	ImGui::DragFloat3("SideWinder Scale", &m_scene->GetInstance(1)->GetScale()[0], 0.1f, 0.01f, 5.0f);
	ImGui::DragFloat3("Bunny Movement", &m_scene->GetInstance(2)->GetPosition()[0], 0.1f, -50.0f, 50.0f);
	ImGui::DragFloat3("Bunny Rotation", &m_scene->GetInstance(2)->GetRotation()[0], 0.1f, -180.0f, 180.0f);
	ImGui::DragFloat3("Bunny Scale", &m_scene->GetInstance(2)->GetScale()[0], 0.1f, 0.01f, 5.0f);

	ImGui::End();
}
//This is a gui of the position, scale and colour of the particles.
void GraphicsProjectApp::IMGUI_Particle() 
{
	ImGui::Begin("Particles Transform");
	ImGui::DragFloat3("Particles Movement", &m_particleTransform[3][0], 0.1f, -50.0f, 50.0f);
	ImGui::DragFloat3("Particles Start Colour", &m_particleStartColour[0], 0.1f, 0, 50.0f);
	ImGui::DragFloat3("Particles End Colour", &m_particleEndColour[0], 0.1f, 0, 50.0f);
	ImGui::DragFloat("Particles Start Scale", &m_particleStartScale, 0.1f, 0.1f, 20.0f);
	ImGui::DragFloat("Particles End Scale", &m_particleEndScale, 0.1f, 0.1f, 10.0f);
	ImGui::End();
}

//This gui is adding a camera to a vector of camera list. It also allows for the player to change to other cameras in scene.
void GraphicsProjectApp::IMGUI_AddCamera()
{
	ImGui::Begin("Camera Control");
	if (ImGui::Button("Add Moving Camera"))
	{
		Camera cam;
		m_multipleCameras.push_back(cam);
	}
	if (ImGui::Button((std::string("Dynamic Camera ") + std::to_string(1)).c_str()))
	{
		camValue = 0;
	}
	if (ImGui::Button("Static Camera X"))
	{
		camValue = 1;
	}
	if (ImGui::Button("Static Camera Y"))
	{
		camValue = 3;
	}
	if (ImGui::Button("Static Camera Z"))
	{
		camValue = 2;
	}
	for (size_t i = 4; i < m_multipleCameras.size(); i++)
	{
		if (ImGui::Button((std::string("Dynamic Camera ") + std::to_string(i - 2)).c_str()))
		{
			camValue = i;
			m_multipleCameras[camValue].SetPerspective(glm::pi<float>() * 0.25f,
				(float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
			m_multipleCameras[camValue].SetLookAt(vec3(10), vec3(0), vec3(0, 1, 0));
		}
	}
	ImGui::End();
}

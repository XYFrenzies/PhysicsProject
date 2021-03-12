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
m_bunnyTransform(), m_bunnyRot(), m_bunnyScale(), m_bunnyTranslation(),
m_dragonTransform(), m_dragonRot(), m_dragonScale(), m_dragonTranslation(),
m_buddhaTransform(), m_buddhaRot(), m_buddhaScale(), m_buddhaTranslation(),
m_lucyTransform(), m_lucyRot(), m_lucyScale(), m_lucyTranslation(),
m_light(), m_planet(), m_projectionMatrix(), m_viewMatrix() 
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

	m_light.colour = { 1, 1, 1 };
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


	m_multipleCameras[camValue].Update(deltaTime);
	IMGUI_Logic();
	IMGUI_Transform();
	IMGUI_AddCamera();
	//Rotates the light direction per time variable.
	float time = getTime();
	m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));
	//Rotates the plane per update.

	//BunnyRotation
	//m_bunnyRot = glm::rotate(m_bunnyRot, deltaTime, glm::vec3(0, 1, 0));
	m_bunnyTransform = m_bunnyTranslation * m_bunnyRot * m_bunnyScale;
	////DragonRotation
	//m_dragonRot = glm::rotate(m_dragonRot, deltaTime, glm::vec3(0, 1, 0));
	m_dragonTransform = m_dragonTranslation * m_dragonRot * m_dragonScale;
	////BuddhaRotation
	//m_buddhaRot = glm::rotate(m_buddhaRot, deltaTime, glm::vec3(0, 1, 0));
	m_buddhaTransform = m_buddhaTranslation * m_buddhaRot * m_buddhaScale;
	////LucyRotation
	//m_lucyRot = glm::rotate(m_lucyRot, deltaTime, glm::vec3(0, 1, 0));
	m_lucyTransform = m_lucyTranslation * m_lucyRot * m_lucyScale;

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
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

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
//#pragma region QuadShader
//
//	//Load the vertex shader from a file
//	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
//
//	//Load the fragment shader from the file
//	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
//	if (!m_simpleShader.link())
//	{
//		printf("Simple Shader had an error: %s\n", m_simpleShader.getLastError());
//		return false;
//	}
//
//#pragma endregion 
//#pragma region BunnyShader
//	m_bunnyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
//
//	//Load the fragment shader from the file
//	m_bunnyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
//	if (!m_bunnyShader.link())
//	{
//		printf("Bunny Shader had an error: %s\n", m_bunnyShader.getLastError());
//		return false;
//	}
//#pragma endregion
//#pragma region DragonShader
//	m_dragonShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
//
//	//Load the fragment shader from the file
//	m_dragonShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
//	if (!m_dragonShader.link())
//	{
//		printf("Dragon Shader had an error: %s\n", m_dragonShader.getLastError());
//		return false;
//	}
//#pragma endregion
//#pragma region BuddhaShader
//	m_buddhaShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
//
//	//Load the fragment shader from the file
//	m_buddhaShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
//	if (!m_buddhaShader.link())
//	{
//		printf("Buddha Shader had an error: %s\n", m_buddhaShader.getLastError());
//		return false;
//	}
//#pragma endregion
//#pragma region LucyShader
//	m_lucyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
//
//	//Load the fragment shader from the file
//	m_lucyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
//	if (!m_lucyShader.link())
//	{
//		printf("Buddha Shader had an error: %s\n", m_lucyShader.getLastError());
//		return false;
//	}
//
//#pragma endregion
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
//#pragma region QuadLogic
//	Mesh::Vertex vertices[4];
//	vertices[0].position = { -0.5f, 0.0f, 0.5f, 1.0f };
//	vertices[1].position = { 0.5f, 0.0f, 0.5f, 1.0f };
//	vertices[2].position = { -0.5f, 0.0f, -0.5f, 1.0f };
//	vertices[3].position = { 0.5f, 0.0f, -0.5f, 1.0f };
//
//	unsigned int indices[6] = { 0,1,2,2,1,3 };
//
//	m_quadMesh.InitialiseQuad();
//
//
//	//We will make the quad 10 units by 10 units
//	m_quadTransform =
//	{
//		10,0,0,0,
//		0,10,0,0,
//		0,0,10,0,
//		0,0,0,1
//	};
//#pragma endregion
//#pragma region BunnyLogic
//
//	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
//	{
//		printf("Bunny Mesh Failed!!!\n");
//		return false;
//	}
//	m_bunnyScale =
//	{
//		0.5f, 0, 0, 0,
//		0, 0.5f, 0, 0,
//		0, 0, 0.5f, 0,
//		0, 0, 0, 1
//	};
//	m_bunnyRot = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
//	m_bunnyTranslation = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 0, -5, 1);
//#pragma endregion
//#pragma region DragonLogic
//
//	if (m_dragonMesh.load("./stanford/Dragon.obj") == false)
//	{
//		printf("Dragon Mesh Failed!!!\n");
//		return false;
//	}
//	m_dragonScale =
//	{
//		0.5f, 0, 0, 0,
//		0, 0.5f, 0, 0,
//		0, 0, 0.5f, 0,
//		0, 0, 0, 1
//	};
//	m_dragonRot = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
//	m_dragonTranslation = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -5, 0, -5, 1);
//#pragma endregion
//#pragma region BuddhaLogic
//	if (m_buddhaMesh.load("./stanford/Buddha.obj") == false)
//	{
//		printf("Buddha Mesh Failed!!!\n");
//		return false;
//	}
//	m_buddhaScale =
//	{
//		0.5f, 0, 0, 0,
//		0, 0.5f, 0, 0,
//		0, 0, 0.5f, 0,
//		0, 0, 0, 1
//	};
//	m_buddhaRot = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
//	m_buddhaTranslation = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -5, 0, 5, 1);
//#pragma endregion
//#pragma region LucyLogic
//
//	if (m_lucyMesh.load("./stanford/Lucy.obj") == false)
//	{
//		printf("Lucy Mesh Failed!!!\n");
//		return false;
//	}
//	m_lucyScale =
//	{
//		0.5f, 0, 0, 0,
//		0, 0.5f, 0, 0,
//		0, 0, 0.5f, 0,
//		0, 0, 0, 1
//	};
//	m_lucyRot = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
//	m_lucyTranslation = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 0, 5, 1);
//#pragma endregion
//#pragma region GridLogic
//	if (!m_gridTexture.load("./textures/numbered_grid.tga"))
//	{
//		printf("Failed to load: numbered_grid.tga\n");
//		return false;
//	}
//
//#pragma endregion
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
//#pragma region Quad
//	//Bind the shader
//	m_textureShader.bind();
//
//	//Bind the transform of the mesh
//	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform;//PVM = Porjectyion View Matrix
//	m_textureShader.bindUniform("ProjectionViewModel", pvm);
//	//Bind the texture to a location of yout choice (0).
//	m_textureShader.bindUniform("diffuseTexture", 0);
//	//Bind the texture to the specified location
//	m_gridTexture.bind(0);
//	m_quadMesh.Draw();
//#pragma endregion
//#pragma region Bunny
//
//	m_bunnyShader.bind();
//	pvm = m_projectionMatrix * a_viewMatrix * m_bunnyTransform;
//	m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
//	m_bunnyShader.bindUniform("MeshFlatColour", glm::vec4(0, 1, 0, 1));
//	//Draw bunny mesh
//	//m_bunnyMesh.draw();
//
//#pragma endregion
//#pragma region Dragon
//
//	m_dragonShader.bind();
//	pvm = m_projectionMatrix * a_viewMatrix * m_dragonTransform;
//	m_dragonShader.bindUniform("ProjectionViewModel", pvm);
//	m_dragonShader.bindUniform("MeshFlatColour", glm::vec4(0, 1, 0, 1));
//#pragma endregion 
//#pragma region Buddha
//
//	m_buddhaShader.bind();
//	pvm = m_projectionMatrix * a_viewMatrix * m_buddhaTransform;
//	m_buddhaShader.bindUniform("ProjectionViewModel", pvm);
//	m_buddhaShader.bindUniform("MeshFlatColour", glm::vec4(0, 1, 0, 1));
//#pragma endregion 
//#pragma region Lucy
//
//	m_lucyShader.bind();
//	pvm = m_projectionMatrix * a_viewMatrix * m_lucyTransform;
//	m_lucyShader.bindUniform("ProjectionViewModel", pvm);
//	m_lucyShader.bindUniform("MeshFlatColour", glm::vec4(0, 1, 0, 1));
//#pragma endregion 
#pragma region Phong
	//Bind the shader
	m_phongShader.bind();

	//Bind the camera position
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(a_viewMatrix)[3]));

	//Bind the light
	m_phongShader.bindUniform("AmbientColour", m_ambientLight);
	m_phongShader.bindUniform("LightColour", m_light.colour);
	m_phongShader.bindUniform("LightDirection", m_light.direction);
#pragma endregion
//#pragma region BunnyPhong
//	//Bind the pvm
//	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//	//Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_bunnyTransform);
//
//	m_bunnyMesh.draw();
//#pragma endregion
//#pragma region DragonPhong
//	pvm = a_projectionMatrix * a_viewMatrix * m_dragonTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//	//Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_dragonTransform);
//
//	m_dragonMesh.draw();
//#pragma endregion
//#pragma region BuddhaPhong
//	pvm = a_projectionMatrix * a_viewMatrix * m_buddhaTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//	//Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_buddhaTransform);
//
//	m_buddhaMesh.draw();
//#pragma endregion
//#pragma region LucyPhong
//	pvm = a_projectionMatrix * a_viewMatrix * m_lucyTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//	//Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_lucyTransform);
//
//	m_lucyMesh.draw();
//#pragma endregion
#pragma region SoulSpear
	m_normalMapShader.bind();

	//Bind the transform
	pvm = a_projectionMatrix * a_viewMatrix * m_spearTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("CameraPosition", m_multipleCameras[camValue].GetPosition());
	m_normalMapShader.bindUniform("AmbientColour", m_ambientLight);
	m_normalMapShader.bindUniform("LightColour", m_light.colour);
	m_normalMapShader.bindUniform("LightDirection",m_light.direction);
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
	m_normalMapShader.bindUniform("LightColour", m_light.colour);
	m_normalMapShader.bindUniform("LightDirection", m_light.direction);
	//Bind the texture to a location of yout choice (0).
	m_normalMapShader.bindUniform("ModelMatrix", m_lightSaberTransform);
	//Draw the nesh
	m_lightSaberMesh.draw();

#pragma endregion

}

void GraphicsProjectApp::IMGUI_Logic()
{
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f, -1.0f);
	ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();
}

void GraphicsProjectApp::IMGUI_Transform()
{
	ImGui::Begin("Object Transform");
	ImGui::DragFloat3("Bunny Movement", &m_bunnyTranslation[3][0], 0.1f, -15.0f, 15.0f);
	ImGui::DragFloat3("Dragon Movement", &m_dragonTranslation[3][0], 0.1f, -15.0f, 15.0f);
	ImGui::DragFloat3("Buddha Movement", &m_buddhaTranslation[3][0], 0.1f, -15.0f, 15.0f);
	ImGui::DragFloat3("Lucy Movement", &m_lucyTranslation[3][0], 0.1f, -15.0f, 15.0f);
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

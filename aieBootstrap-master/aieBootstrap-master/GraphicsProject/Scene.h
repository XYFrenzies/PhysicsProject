#pragma once
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include "OBJMesh.h"

//This file contains the objects and lighting in the scene, the updates and the drawings of the objects.
//Created by Benjamin McDonald on the 18th March 2021
//Last accessed: 1st April 2021
class Camera;
class Instance;
class GraphicsProjectApp;
const int MAX_LIGHTS = 4;

//A struct of a light in memory
struct Light 
{
	//The initiale constructor
	Light() 
	{
		m_direction = glm::vec3(1);
		m_color = glm::vec3(1);
	}
	//Overloaded constructor
	Light(glm::vec3 a_position, glm::vec3 a_color, float a_intensity) 
	{
		m_direction = a_position;
		m_color = a_color * a_intensity;
	}
	//The direction and colour of the object.
	glm::vec3 m_direction;
	glm::vec3 m_color;
};

class Scene
{
public:
	Scene(Camera* a_camera, glm::vec2 a_windowSize, 
		Light& a_light, glm::vec3 a_ambientLight);//The scene takes in a camera, window w and h, a light and a ambient light.
	~Scene();
	void AddInstance(Instance* a_instance);//Adds an instance of the object.
	void Draw();
	Instance* GetInstance(int index) { return m_instances[index]; }//Returns the instance in scene.
	int GetNumLights() { return (int)m_pointLights.size(); }//Returns the amount of lights in scene.
	glm::vec3* GetPointLightPositions() { return &m_pointLightPos[0]; }//Gets the position of the point lights
	glm::vec3* GetPointLightColours() { return &m_pointLightCol[0]; }//Gets the colour of the point lights
	std::vector<Light>& GetPointLights() { return m_pointLights; }//Returns the point light in the array.

	//Camera getters and setters
	Camera* GetCamera() { return m_camera; }
	Camera* SetCamera(Camera* a_cam) { return m_camera = a_cam; }

	
	glm::vec2 GetWindowSize() { return m_windowSize; }//Window height and width
	//The types of lights within the scene. 
	Light& GetLight() { return m_light; }
	glm::vec3 GetAmbientLight() { return m_ambientLight; }
	glm::vec3 SetAmbientLight(glm::vec3 a_ambientLight) { return m_ambientLight = a_ambientLight; }
protected:
	friend class GraphicsProjectApp;//Giving the Project access to variables within this scene.
	Camera* m_camera;
	glm::vec2 m_windowSize;
	glm::vec3 m_ambientLight;
	Light m_light;
	Light m_sunlight;
	std::vector<Light> m_pointLights;
	std::vector<Instance*> m_instances;

	glm::vec3 m_pointLightPos[MAX_LIGHTS];
	glm::vec3 m_pointLightCol[MAX_LIGHTS];
};


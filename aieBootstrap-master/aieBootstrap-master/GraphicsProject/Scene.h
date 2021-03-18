#pragma once
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include "OBJMesh.h"

class Camera;
class Instance;
class GraphicsProjectApp;
const int MAX_LIGHTS = 4;

struct Light 
{
	Light() 
	{
		m_direction = glm::vec3(1);
		m_color = glm::vec3(1);
	}
	Light(glm::vec3 a_position, glm::vec3 a_color, float a_intensity) 
	{
		m_direction = a_position;
		m_color = a_color * a_intensity;
	}
	glm::vec3 m_direction;
	glm::vec3 m_color;
};

class Scene
{
public:
	Scene(Camera* a_camera, glm::vec2 a_windowSize, 
		Light& a_light, glm::vec3 a_ambientLight);
	~Scene();
	void AddInstance(Instance* a_instance);
	void Draw();
	Instance* GetInstance(int index) { return m_instances[index]; }
	int GetNumLights() { return (int)m_pointLights.size(); }
	glm::vec3* GetPointLightPositions() { return &m_pointLightPos[0]; }
	glm::vec3* GetPointLightColours() { return &m_pointLightCol[0]; }
	std::vector<Light>& GetPointLights() { return m_pointLights; }


	Camera* GetCamera() { return m_camera; }
	Camera* SetCamera(Camera* a_cam) { return m_camera = a_cam; }
	glm::vec2 GetWindowSize() { return m_windowSize; }
	Light& GetLight() { return m_light; }
	glm::vec3 GetAmbientLight() { return m_ambientLight; }
protected:
	friend class GraphicsProjectApp;
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


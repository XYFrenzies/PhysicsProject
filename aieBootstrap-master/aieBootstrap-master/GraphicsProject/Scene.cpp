#include "Scene.h"
#include "Instance.h"
Scene::Scene(Camera* a_camera, glm::vec2 a_windowSize, Light& a_light, glm::vec3 a_ambientLight)
	: m_camera(a_camera), m_windowSize(a_windowSize), m_light(a_light), m_ambientLight(a_ambientLight), 
	m_pointLightCol(), m_pointLightPos()
{
	m_ambientLight = a_ambientLight;
}

Scene::~Scene()
{
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		delete(*i);
	}
}

void Scene::AddInstance(Instance* a_instance)
{
	//For a new instance, it is pushed back to the end of the array.
	m_instances.push_back(a_instance);
}

void Scene::Draw()
{
	//Updates the amount of lights in scene and draws their direction and colour.
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size() ; i++)
	{
		m_pointLightPos[i] = m_pointLights[i].m_direction;
		m_pointLightCol[i] = m_pointLights[i].m_color;
	}
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		//Draws the objects in the scene with the information of the directional lights and colours.
		Instance* instance = *i;
		instance->Draw(this);
	}
}



#pragma once
#include "Application.h"
#include <glm/glm.hpp>
//Created by Benjamin McDonald on the 10th March 2021
//Last accessed: 1st April 2021
class Planet
{
public:
	Planet(glm::vec3 a_position, float a_rotationSpeed, float a_radius, float a_distFromCenter,
		glm::vec4 a_colour);
	~Planet();

	void Update(float a_deltaTime);
	void Draw();
	void MakeGizmo();
	glm::vec3 GetPosition() { return m_position; }

private:

protected:
	glm::vec3 m_position;
	glm::vec4 m_colour;
	float m_radius;
	float m_rotationSpeed;
	float m_distFromCenter;
	aie::Application* app;
};


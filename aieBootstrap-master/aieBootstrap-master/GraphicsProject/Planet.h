#pragma once
#include "Application.h"
#include <glm/glm.hpp>
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


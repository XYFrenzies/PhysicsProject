#include "Planet.h"
#include <glm/glm.hpp>

#include "Gizmos.h"
Planet::Planet(glm::vec3 a_position, float a_rotationSpeed, float a_radius, float a_distFromCenter,
	glm::vec4 a_colour) : app()
{
	m_position = a_position;
	m_radius = a_radius;
	m_rotationSpeed = a_rotationSpeed;
	m_distFromCenter = a_distFromCenter;
	m_colour = a_colour;

}

Planet::~Planet()
{
}

void Planet::Update(float a_deltaTime)
{
	float time = app->getTime();
	m_position.x = m_radius * glm::cos(time * m_rotationSpeed) * m_distFromCenter;
	m_position.z = m_radius * glm::sin(time * m_rotationSpeed) * m_distFromCenter;
}
void Planet::MakeGizmo()
{
	aie::Gizmos::addSphere(m_position, m_radius, 12, 12, m_colour);
}
void Planet::Draw()
{
}

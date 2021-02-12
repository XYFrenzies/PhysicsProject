#include "Sphere.h"
#include <Gizmos.h>
Sphere::Sphere(glm::vec2 a_pos, glm::vec2 a_vel, float a_rot, float a_mass, 
	float a_radius, glm::vec4 a_colour) : Rigidbody(ShapeType::SPHERE, a_pos, a_vel, a_mass, a_rot)
{
	m_radius = a_radius;
	m_colour = a_colour;
	m_moment = 0.5f * a_mass * a_radius * a_radius;
}

Sphere::~Sphere()
{
}

void Sphere::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	Rigidbody::FixedUpdate(a_gravity, a_timeStep);

	float cos = cosf(m_rot);
	float sin = sinf(m_rot);
	m_localX = glm::normalize(glm::vec2(cos, sin)); 
	m_localY = glm::normalize(glm::vec2(-sin, cos));

}

void Sphere::MakeGizmo()
{

	glm::vec2 end = glm::vec2(std::cos(m_rot), std::sin(m_rot)) * m_radius;


	aie::Gizmos::add2DCircle(m_pos, m_radius, 100, m_colour);
	aie::Gizmos::add2DLine(m_pos, m_pos + end, glm::vec4(1, 1, 1, 1));
}

bool Sphere::IsInside(glm::vec2 a_point)
{
	return glm::distance(a_point, GetPosition()) <= GetRadius();
}


#include "Sphere.h"
#include <Gizmos.h>
Sphere::Sphere(glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, 
	float a_radius, glm::vec4 a_colour) : Rigidbody(ShapeType::SPHERE, a_pos, a_vel, a_mass, 0)
{
	m_radius = a_radius;
	m_colour = a_colour;
}

Sphere::~Sphere()
{
}

void Sphere::MakeGizmo()
{
	glm::vec2 end = glm::vec2(std::cos(m_rot), std::sin(m_rot)) * m_radius;


	aie::Gizmos::add2DCircle(m_pos, m_radius, 12, m_colour);
	aie::Gizmos::add2DLine(m_pos, m_pos + end, glm::vec4(1, 1, 1, 1));
}

bool Sphere::IsInside(glm::vec2 a_point)
{
	return glm::distance(a_point, GetPosition()) <= GetRadius();
}


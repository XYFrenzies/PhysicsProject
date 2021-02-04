#include "Sphere.h"
#include <Gizmos.h>
Sphere::Sphere(glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, 
	float a_radius, glm::vec4 a_colour) : Rigidbody(SPHERE, a_pos, a_vel, 0, a_mass)
{
	m_radius = a_radius;
	m_mass = a_mass;
	m_colour = a_colour;
}

Sphere::~Sphere()
{
}

void Sphere::MakeGizmo()
{
	aie::Gizmos::add2DCircle(m_pos, m_radius, 12, m_colour);
}

bool Sphere::CheckCollision(PhysicsObject* pOther)
{
	Sphere* pSphere = dynamic_cast<Sphere*>(pOther);
	if (pSphere != nullptr)
	{
		float dist = glm::distance(GetPosition(), pSphere->GetPosition());
		if (GetRadius() + pSphere->GetRadius() > dist)
			return true;
	}
	return false;
}

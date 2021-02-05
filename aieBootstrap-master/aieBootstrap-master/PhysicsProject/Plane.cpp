#include "Plane.h"
#include <Gizmos.h>
Plane::Plane(glm::vec2 a_norm, float a_dist) : PhysicsObject(PLANE)
{
	m_norm = a_norm;
	m_distToOrigin = a_dist;
	m_colour = glm::vec4(0, 1, 0, 1);
}

Plane::Plane() : PhysicsObject(PLANE)
{
	m_norm = glm::vec2(0, 1);
	m_distToOrigin = 0;
	m_colour = glm::vec4(1, 1, 1, 1);
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
}

void Plane::MakeGizmo()
{
	float lineSegmentLength = 300.0f;
	glm::vec2 centerPoint = m_norm * m_distToOrigin;

	glm::vec2 parrallel(m_norm.y, -m_norm.x);
	glm::vec4 colourFade = m_colour;
	colourFade.a = 0;
	glm::vec2 start = centerPoint + (parrallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parrallel * lineSegmentLength);

	aie::Gizmos::add2DTri(start, end, start - GetNormal() * 10.0f, 
		GetColour(), GetColour(), colourFade);
	aie::Gizmos::add2DTri(end, end - GetNormal() * 10.0f,
		start - GetNormal() * 10.0f, GetColour(), colourFade, colourFade);
}

#pragma once
#include "Rigidbody.h"
class Sphere : public Rigidbody
{
public:
	Sphere(glm::vec2 a_pos, glm::vec2 a_vel, float a_rot, 
		float a_mass, float a_radius, glm::vec4 a_colour);
	~Sphere();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);

	virtual void MakeGizmo();
	virtual bool IsInside(glm::vec2 a_point);

	float GetRadius() { return m_radius; };
	glm::vec4 GetColour() { return m_colour; }
	glm::vec2 GetLocalX() { return m_localX; }
	glm::vec2 GetLocalY() { return m_localY; }
private:
protected:
	glm::vec2 m_extents;
	glm::vec2 m_localX;
	glm::vec2 m_localY;

	float m_radius;
	glm::vec4 m_colour;
};


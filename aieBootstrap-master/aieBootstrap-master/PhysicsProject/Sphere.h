#pragma once
#include "Rigidbody.h"
class Sphere : public Rigidbody
{
public:
	Sphere(glm::vec2 a_pos, glm::vec2 a_vel, 
		float a_mass, float a_radius, glm::vec4 a_colour);
	~Sphere();

	virtual void MakeGizmo();


	float GetRadius() { return m_radius; };
	glm::vec4 GetColour() { return m_colour; }

private:
protected:
	float m_radius;
	glm::vec4 m_colour;
};


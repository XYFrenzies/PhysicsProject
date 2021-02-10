#pragma once
#include "Rigidbody.h"
class Box : public Rigidbody
{
public:
	Box(glm::vec2 a_pos, glm::vec2 a_vel, 
		float a_rot, float a_mass, float a_width, float a_height);
	Box(glm::vec2 a_pos, glm::vec2 a_vel,
		float a_rot, float a_mass, float a_width, float a_height, glm::vec4 a_colour);
	~Box();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void MakeGizmo();

	bool CheckBoxCorners(const Box& a_box, glm::vec2& a_contact,
	int& a_numContacts, float& a_pen, glm::vec2& a_edgeNormal);

	float GetWidth() { return m_extents.x * 2; }
	float GetHeight() { return m_extents.y * 2; }
	glm::vec2 GetExtents() const { return m_extents; }

	glm::vec2 GetLocalX() { return m_localX; }
	glm::vec2 GetLocalY() { return m_localY; }
	
	glm::vec4 GetColour() { return m_colour; }

private:
protected:
	glm::vec2 m_extents;//Half length of objects.
	glm::vec4 m_colour;//Colour of object

	//These two values will store the local x and y axes of the 
	//box based on its angle of rotation.
	glm::vec2 m_localX;
	glm::vec2 m_localY;
};


#pragma once
#include "PhysicsObject.h"
//This is a one-sided object that extgended infinitely 
//along boith its edge and backwards from its normal direction
class Rigidbody;
class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 a_norm, float a_dist); //The normal and the distance.
	Plane();
	~Plane();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void Draw() {};
	virtual void Debug() {};
	virtual void ResetPosition() {};
	virtual void MakeGizmo();

	void ResolveCollision(Rigidbody* a_otherActor, glm::vec2 a_contact);

	glm::vec2 GetNormal() { return m_norm; }
	float GetDistance() { return m_distToOrigin; }
	glm::vec4 GetColour() { return m_colour; }

private:
protected:
	glm::vec2 m_norm;//Member Normal
	glm::vec4 m_colour;
	float m_distToOrigin; //Member Distance

};


#pragma once
#include "PhysicsObject.h"
class Rigidbody : public PhysicsObject
{
public:

	Rigidbody(ShapeType a_shapeID, glm::vec2 a_pos, 
		glm::vec2 a_vel, float a_mass, float a_rot);
	~Rigidbody();
	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void Debug() {};
	void ApplyForce(glm::vec2 a_force);
	void ApplyForceToOther(Rigidbody* a_otherActor, glm::vec2 a_force);
	void ResolveCollision(Rigidbody* a_otherActor);
	virtual bool CheckCollision(PhysicsObject* pOther) = 0;

	glm::vec2 GetPosition() { return m_pos; }
	glm::vec2 GetVelocity() { return m_vel; }
	float GetMass() { return m_mass; }
	float GetRotation() { return m_rot; }
private:
protected:
	glm::vec2 m_pos;//Position
	glm::vec2 m_vel;//Velocity

	float m_mass;
	float m_rot;//Rotation
};


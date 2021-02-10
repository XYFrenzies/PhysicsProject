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
	void ApplyForce(glm::vec2 a_force, glm::vec2 a_pos);
	void ResolveCollision(Rigidbody* a_otherActor,
		glm::vec2 a_contact, glm::vec2* a_colNormal = nullptr);
	//virtual bool CheckCollision(PhysicsObject* pOther) = 0;

	glm::vec2 GetPosition() const { return m_pos; }
	glm::vec2 GetVelocity() { return m_vel; }
	float GetMass() { return m_mass; }
	float GetRotation() { return m_rot; }
	float GetMoment() { return m_moment; }
	float GetAngularVelocity() { return m_angVel; }

	float SetRotation(float a_rot) {return m_rot = a_rot; }
private:
protected:
	glm::vec2 m_pos;//Position
	glm::vec2 m_vel;//Velocity

	float m_mass;
	float m_rot;//Rotation
	float m_angVel;//Angular Velocity
	float m_moment;
};


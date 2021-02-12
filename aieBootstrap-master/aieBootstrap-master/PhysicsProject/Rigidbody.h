#pragma once
#include "PhysicsObject.h"
#include <list>
#include <functional>
#include <iostream>
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
		glm::vec2 a_contact, glm::vec2* a_colNormal = nullptr, float a_pen = 0);
	//virtual bool CheckCollision(PhysicsObject* pOther) = 0;

	glm::vec2 GetPosition() const { return m_pos; }
	glm::vec2 GetVelocity() { 
		return m_vel; }
	float GetMass() { return m_isKinematic ? INT_MAX : m_mass; }
	float GetRotation() { return m_rot; }
	float GetMoment() 
	{ 
		return m_isKinematic ? INT_MAX : m_moment; 
	}
	float GetAngularVelocity() { 
		return m_angVel; }

	float SetRotation(float a_rot) { return m_rot = a_rot; }
	glm::vec2 SetPosition(glm::vec2 a_newPos) { return m_pos = a_newPos; }


	float GetLinearDrag() { return m_linDrag; }
	float GetAngularDrag() { return m_angDrag; }

	glm::vec2 ToWorld(glm::vec2 a_localPos);

	std::function<void(PhysicsObject*)> m_collisionCallback;

	void TriggerEnter(PhysicsObject* a_otherActor);
	std::function<void(PhysicsObject*)> m_triggerEnter;
	std::function<void(PhysicsObject*)> m_triggerExit;
	bool isTrigger() { return m_isTrigger; }
	bool SetTrigger(bool a_state) { return m_isTrigger = a_state; }

private:
protected:
	bool m_isTrigger;
	std::list<PhysicsObject*> m_objInside;
	std::list<PhysicsObject*> m_objInsideThisFrame;

	glm::vec2 m_pos;//Position
	glm::vec2 m_vel;//Velocity

	float m_mass;
	float m_rot;//Rotation
	float m_angVel;//Angular Velocity
	float m_moment;

	float m_linDrag;
	float m_angDrag;

	//These two values will store the local x and y axes of the 
	//rigidbody/box based on its angle of rotation.
	glm::vec2 m_localX;
	glm::vec2 m_localY;
};


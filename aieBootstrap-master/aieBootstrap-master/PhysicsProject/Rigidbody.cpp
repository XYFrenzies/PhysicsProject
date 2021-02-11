#include "Rigidbody.h"
#include "PhysicsScene.h"
#include <iostream>
#define MIN_LINEAR_THRESHOLD 0.001f;
#define MIN_ANGULAR_THRESHOLD 0.001f;

Rigidbody::Rigidbody(ShapeType a_shapeID, glm::vec2 a_pos, 
	glm::vec2 a_vel, float a_mass, float a_rot) : PhysicsObject(a_shapeID)
{
	m_pos = a_pos;
	m_vel = a_vel;
	m_mass = a_mass;
	m_rot = a_rot;
	m_angVel = 0;
	m_isKinematic = false;
	m_isTrigger = false;
	m_elasticity = 0.8f;
	m_linDrag = 0.3f;
	m_angDrag = 0.3f;

}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	//Check if it is a trigger
	if (m_isTrigger)
	{
		//This will let us check every object that is inside a trigger
		//object and call triggerEnter on if they havent registered
		//inside the trigger this frame, they must have exited so we can remove
		//them from the list and them call triggerExit
		for (auto it = m_objInside.begin(); it != m_objInside.end(); it++)
		{
			if (std::find(m_objInsideThisFrame.begin(), 
				m_objInsideThisFrame.end(), *it) == 
				m_objInsideThisFrame.end())
			{
				if (m_triggerExit)
					m_triggerExit(*it);
				it = m_objInside.erase(it);
				if (it == m_objInside.end())
					break;

			}
		}
	}
	//Clear the list now for the next frame.
	m_objInsideThisFrame.clear();
	if (m_isKinematic)
	{
		m_vel = glm::vec2(0);
		m_angVel = 0;
		return;
	}
	m_vel -= m_vel * m_linDrag * a_timeStep;
	m_angVel -= m_angVel * m_angDrag * a_timeStep;

	if (glm::length(m_vel) < 0.001f)
	{
		if (glm::length(m_vel) < glm::length(a_gravity) * m_linDrag * a_timeStep)
			m_vel = glm::vec2(0);

	}
	if (std::abs(m_angVel) < 0.001f)
	{
		m_angVel = 0.0f;
	}


	ApplyForce(a_gravity * GetMass() * a_timeStep, glm::vec2(0));
	m_pos += GetVelocity() * a_timeStep;

	m_rot += m_angVel * a_timeStep;
}

void Rigidbody::ApplyForce(glm::vec2 a_force, glm::vec2 a_pos)
{
	m_vel += a_force / GetMass();
	m_angVel += (a_force.y * a_pos.x - a_force.x * a_pos.y) / GetMoment();
}



void Rigidbody::ResolveCollision(Rigidbody* a_otherActor, 
	glm::vec2 a_contact, glm::vec2* a_colNormal, float a_pen)
{
	//Register that these two objects ave overlapped this frame
	m_objInsideThisFrame.push_back(a_otherActor);
	a_otherActor->m_objInsideThisFrame.push_back(this);

	//Find the vector between their centers, or use the provided
	//direction of force, and make sure it's normalised.
	//This below function determines if the first value has a value, do the left value of the ":"
	//if it doesnt have a value, the value on the right is done.
	glm::vec2 normal = glm::normalize(a_colNormal ? *a_colNormal :
		a_otherActor->GetPosition() - GetPosition());

	//Get the vector perpendicular to the collision normal.
	glm::vec2 perpendicularColNorm(normal.y, -normal.x);
	//These are applied to the radius from axis to the application of force
	float radius1 = glm::dot(a_contact - m_pos, -perpendicularColNorm);
	float radius2 = glm::dot(a_contact - a_otherActor->GetPosition(), 
		perpendicularColNorm);

	//Velocity of the contact point on the object
	float cp_vel1 = glm::dot(m_vel, normal) - radius1 * m_angVel;
	//Velocity of contact point of the other object
	float cp_vel2 = glm::dot(a_otherActor->GetVelocity(), normal) + 
		radius2 * a_otherActor->m_angVel;

	if (cp_vel1 > cp_vel2)//They are moving closer
	{
		//This will calculate the effective mass at the contact point of each object
		//How much it will move due to the forces applied
		float mass1 = 1.0f / (1.0f / m_mass + (radius1 * radius1) / GetMoment());
		float mass2 = 1.0f / (1.0f / a_otherActor->m_mass +
			(radius2 + radius2) / a_otherActor->GetMoment());

		float e = (m_elasticity + a_otherActor->GetElasticity()) / 2.0f;
		glm::vec2 impact = (1.0f + e) * mass1 * mass2 / (mass1 + mass2) * (cp_vel1 - cp_vel2) * normal;

		if (!m_isTrigger && !a_otherActor->isTrigger())
		{
			ApplyForce(-impact, a_contact - m_pos);
			a_otherActor->ApplyForce(impact, a_contact - a_otherActor->GetPosition());

			if (m_collisionCallback != nullptr)
				m_collisionCallback(a_otherActor);
			if (a_otherActor->m_collisionCallback)
				a_otherActor->m_collisionCallback(this);
		}
		else
		{
			TriggerEnter(a_otherActor);
			a_otherActor->TriggerEnter(this);
		}



		if (a_pen > 0)
		{
			PhysicsScene::ApplyContactForces(this, a_otherActor, normal, a_pen);
		}

	}
}

glm::vec2 Rigidbody::ToWorld(glm::vec2 a_localPos)
{
	return m_pos + m_localX * a_localPos.x + m_localY * a_localPos.y;
}

void Rigidbody::TriggerEnter(PhysicsObject* a_otherActor)
{
	if (m_isTrigger && std::find(m_objInside.begin(),
		m_objInside.end(), a_otherActor) == m_objInside.end())
	{
		m_objInside.push_back(a_otherActor);
		if (m_triggerEnter != nullptr)
		{
			m_triggerEnter(a_otherActor);
		}
	}
}

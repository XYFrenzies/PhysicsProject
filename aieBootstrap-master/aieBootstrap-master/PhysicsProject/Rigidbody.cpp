#include "Rigidbody.h"

Rigidbody::Rigidbody(ShapeType a_shapeID, glm::vec2 a_pos, 
	glm::vec2 a_vel, float a_mass, float a_rot) : PhysicsObject(a_shapeID)
{
	m_pos = a_pos;
	m_vel = a_vel;
	m_mass = a_mass;
	m_rot = a_rot;
	m_angVel = 0;
	m_isKinematic = false;
	m_elasticity = 0.8f;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
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
	glm::vec2 a_contact, glm::vec2* a_colNormal)
{
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

		ApplyForce(-impact, a_contact - m_pos);
		a_otherActor->ApplyForce(impact, a_contact - a_otherActor->GetPosition());
	}
}

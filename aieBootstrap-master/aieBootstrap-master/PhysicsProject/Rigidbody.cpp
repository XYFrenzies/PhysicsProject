#include "Rigidbody.h"

Rigidbody::Rigidbody(ShapeType a_shapeID, glm::vec2 a_pos, 
	glm::vec2 a_vel, float a_mass, float a_rot) : PhysicsObject(a_shapeID)
{
	m_pos = a_pos;
	m_vel = a_vel;
	m_mass = a_mass;
	m_rot = a_rot;

}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	ApplyForce(a_gravity * GetMass() * a_timeStep);
	m_pos += GetVelocity() * a_timeStep;
}

void Rigidbody::ApplyForce(glm::vec2 a_force)
{
	m_vel += a_force / GetMass();
}

void Rigidbody::ApplyForceToOther(Rigidbody* a_otherActor, glm::vec2 a_force)
{
	ApplyForce(-a_force);
	a_otherActor->ApplyForce(a_force);
}

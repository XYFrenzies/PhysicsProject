#include "Plane.h"
#include <Gizmos.h>
#include "Rigidbody.h"
Plane::Plane(glm::vec2 a_norm, float a_dist) : PhysicsObject(ShapeType::PLANE)
{
	m_norm = a_norm;
	m_distToOrigin = a_dist;
	m_colour = glm::vec4(0, 1, 0, 1);
	m_isKinematic = true;
	m_elasticity = 1.0f;
}

Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	m_norm = glm::normalize(glm::vec2(0, 1));
	m_distToOrigin = 0;
	m_colour = glm::vec4(1, 1, 1, 1);
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
}

void Plane::MakeGizmo()
{
	float lineSegmentLength = 300.0f;
	glm::vec2 centerPoint = m_norm * m_distToOrigin;

	glm::vec2 parrallel(m_norm.y, -m_norm.x);
	glm::vec4 colourFade = m_colour;
	colourFade.a = 0;
	glm::vec2 start = centerPoint + (parrallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parrallel * lineSegmentLength);

	aie::Gizmos::add2DTri(start, end, start - GetNormal() * 10.0f, 
		GetColour(), GetColour(), colourFade);
	aie::Gizmos::add2DTri(end, end - GetNormal() * 10.0f,
		start - GetNormal() * 10.0f, GetColour(), colourFade, colourFade);
}

void Plane::ResolveCollision(Rigidbody* a_otherActor, glm::vec2 a_contact)
{
	//The position at which we'll apply the force, relative to the other objects Center of Mass 
	glm::vec2 localContact = a_contact - a_otherActor->GetPosition();
	
	//The plane never moves, so the relative 
	//velocity is just the a_otherActor's velocity at the contact point 
	glm::vec2 vRel = a_otherActor->GetVelocity() + a_otherActor->GetAngularVelocity() *
		glm::vec2(-localContact.y, localContact.x);
	float velocityIntoPlane = glm::dot(vRel, m_norm);

	//This can be left as perfect elasticity for the moment 
	float elasticity = (m_elasticity + a_otherActor->GetElasticity()) / 2.0f;

	//This is the perpendicular distance we apply the 
	//force at relative to the center of mass: Torque = F * r.
	float r = glm::dot(localContact, glm::vec2(m_norm.y, m_norm.x));

	//This will work out the 'effective mass' - a combination of the moment of 
	//inertia and mass, it will tell how much the contact point velocitywill change the force we apply
	float mass0 = 1.0f / (1.0f / a_otherActor->GetMass() + (r + r) / a_otherActor->GetMoment());

	//The plane does not move (Static) sowe only use the other actor's velocity
	float j = -(1 + elasticity) * velocityIntoPlane * mass0; 
	glm::vec2 force = m_norm * j;
	a_otherActor->ApplyForce(force, a_contact - a_otherActor->GetPosition());
}

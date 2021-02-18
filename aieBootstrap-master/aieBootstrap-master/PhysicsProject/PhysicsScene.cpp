#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include <list>
#include <iostream>

//Function pointer array for handing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn colFuncArray[] =
{
	PhysicsScene::Plane2Plane,  PhysicsScene::Plane2Sphere,  PhysicsScene::Plane2Box,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere, PhysicsScene::Sphere2Box,
	PhysicsScene::Box2Plane,	PhysicsScene::Box2Sphere,    PhysicsScene::Box2Box
};

PhysicsScene::PhysicsScene() : m_timeStep(0.01f),  m_gravity(glm::vec2(0,0))
{
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
		delete pActor;
}

void PhysicsScene::AddActor(PhysicsObject* a_actor)
{
	if(a_actor != nullptr)
		m_actors.push_back(a_actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* a_actor)
{
	auto it = std::find(m_actors.begin(), m_actors.end(), a_actor);
	if (it != m_actors.end())
		m_actors.erase(it);
}

PhysicsObject* PhysicsScene::GetActor(PhysicsObject* a_actor)
{
	for (size_t i = 0; i < m_actors.size() - 1; i++)
	{
		if (m_actors[i] = a_actor)
			return m_actors[i];
	}

	return nullptr;
}

void PhysicsScene::Update(float dt)
{
	static std::list<PhysicsObject*> objs;
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;
	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;

		CheckCollisions();
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->MakeGizmo();
	}
}

void PhysicsScene::DebugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->Debug();
		count++;
	}
}

void PhysicsScene::CheckCollisions()
{
	size_t actorCount = m_actors.size();
	for (size_t outer = 0; outer < actorCount - 1; outer++)
	{
		for (size_t inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* objOuter = m_actors[outer];
			PhysicsObject* objInner = m_actors[inner];
			int shapeID_out = (int)objOuter->GetShapeID();
			int shapeID_in = (int)objInner->GetShapeID();
			
			//This will check to ensure we do not include the joints
			if (shapeID_in >= 0 && shapeID_out >= 0)
			{
				//Uses our function pointer (fn)
				int funcIndex = (shapeID_out * (int)ShapeType::SHAPE_COUNT) + shapeID_in;
				fn colFuncPtr = colFuncArray[funcIndex];
				if (colFuncPtr != nullptr)
				{
					//Check if the collision occurs.
					colFuncPtr(objOuter, objInner);
				}
			}
		}
	}
}

void PhysicsScene::ApplyContactForces(Rigidbody* a_actor1, Rigidbody* a_actor2, glm::vec2 a_colNorm, float a_pen)
{
	if ((a_actor1 && a_actor1->isTrigger()) || (a_actor2 && a_actor2->isTrigger()))
		return;

	//Checking if our object is kinematic or not.
	float bodyToMass = a_actor2 ? a_actor2->GetMass() : INT_MAX;
	float body1Factor = bodyToMass / (a_actor1->GetMass() + bodyToMass);

	if(a_actor1 != nullptr)
		a_actor1->SetPosition(a_actor1->GetPosition() - body1Factor * a_colNorm * a_pen);
	
	if (a_actor2)
	{
		a_actor2->SetPosition(a_actor2->GetPosition() + (1 - body1Factor) * a_colNorm * a_pen);
	}


}

bool PhysicsScene::Plane2Plane(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* objPlane, PhysicsObject* objSphere)
{
	return Sphere2Plane(objSphere, objPlane);
}

bool PhysicsScene::Plane2Box(PhysicsObject* objPlane, PhysicsObject* objBox)
{
	Plane* plane = dynamic_cast<Plane*>(objPlane);
	Box* box = dynamic_cast<Box*>(objBox);


	//If successful we test for collision
	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		
		//Get a representation point on the plane
		glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();
		//Check all the corners for a collision with the plane
		for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
		{
			for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
			{
				//Get the position of the corners in world space.
				glm::vec2 p = box->GetPosition() + x * box->GetLocalX() + y * box->GetLocalY();
				float distFromPlane = glm::dot(p - planeOrigin, plane->GetNormal());
				//This is the total velocity ofd the points in world space
				glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();
				glm::vec2 pointVelocity = box->GetVelocity() + box->GetAngularVelocity() 
					* glm::vec2(-displacement.y, displacement.x);
				//This is the amount of the point velocity into the plane 
				float velocityIntoPlane = glm::dot(pointVelocity, plane->GetNormal());
				//Moving further in, we need to resolve the collision
				if (distFromPlane < 0 && velocityIntoPlane <= 0)
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}
		//If we hit it will normall result as one or two corners touching a plane
		if (numContacts > 0)
		{
			plane->ResolveCollision(box, contact / (float)numContacts);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* objSphere, PhysicsObject* objPlane)
{
	Sphere* sphere = dynamic_cast<Sphere*>(objSphere);
	Plane* plane = dynamic_cast<Plane*>(objPlane);


	//If successful and they both have a value.
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 colNorm = plane->GetNormal();
		float sphereToPlane = glm::dot(sphere->GetPosition(), 
			colNorm) - plane->GetDistance();
		float intersec = sphere->GetRadius() - sphereToPlane;//Interaction 
		float velOutOfPlane = glm::dot(sphere->GetVelocity(), colNorm);
		if (intersec > 0 && velOutOfPlane < 0)
		{

			glm::vec2 contact = sphere->GetPosition() + (colNorm * -sphere->GetRadius());
			plane->ResolveCollision(sphere, contact);

/*			glm::vec2 holdVel = sphere->GetVelocity();
			sphere->ApplyForce(-holdVel * sphere->GetMass()
				+ glm::vec2(holdVel.x * 4, -holdVel.y * 4));*///Gives the opposite effect of what the object will have.
													//This now bounces in the correct movement
			return true;
		}
		
	}

	return false;
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* objSphere1, PhysicsObject* objSphere2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(objSphere1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(objSphere2);

	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float dist = glm::distance(sphere1->GetPosition(), sphere2->GetPosition());

		float overlap = sphere1->GetRadius() + sphere2->GetRadius() - dist;
		if (overlap > 0)
		{
			sphere1->ResolveCollision(sphere2, 0.5f * (sphere1->GetPosition() + sphere2->GetPosition()), nullptr, overlap);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Sphere2Box(PhysicsObject* objSphere, PhysicsObject* objBox)
{
	Sphere* sphere = dynamic_cast<Sphere*>(objSphere);
	Box* box = dynamic_cast<Box*>(objBox);

	if (box != nullptr && sphere != nullptr)
	{
		//Transform the circle into the box's coordinate space
		glm::vec2 circlePosWorld = sphere->GetPosition() - box->GetPosition();
		glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->GetLocalX()),
			glm::dot(circlePosWorld, box->GetLocalY()));

		//Find the closest point to the circle's center on the box by clamping the
		//coordinates in the box space to the box's extents.
		glm::vec2 closestPointOnTheBox = circlePosBox;
		glm::vec2 extents = box->GetExtents();
		if (closestPointOnTheBox.x < -extents.x) closestPointOnTheBox.x = -extents.x;
		if (closestPointOnTheBox.x > extents.x)  closestPointOnTheBox.x = extents.x;
		if (closestPointOnTheBox.y < -extents.y) closestPointOnTheBox.y = -extents.y;
		if (closestPointOnTheBox.y > extents.y)  closestPointOnTheBox.y = extents.y;

		//Now covert it back into world coordinates.
		glm::vec2 closestPointOnBoxWorld = box->GetPosition() + closestPointOnTheBox.x *
			box->GetLocalX() + closestPointOnTheBox.y * box->GetLocalY();
		glm::vec2 circleToBox = sphere->GetPosition() - closestPointOnBoxWorld;
		float penetration = sphere->GetRadius() - glm::length(circleToBox);
		if (penetration > 0)
		{
			glm::vec2 direction = glm::normalize(circleToBox);
			glm::vec2 contact = closestPointOnBoxWorld;
			box->ResolveCollision(sphere, contact, &direction, penetration);
		}
	}
	return false;
}

bool PhysicsScene::Box2Plane(PhysicsObject* objBox, PhysicsObject* objPlane)
{
	return Plane2Box(objPlane, objBox);
}

bool PhysicsScene::Box2Sphere(PhysicsObject* objBox, PhysicsObject* objSphere)
{
	return Sphere2Box(objSphere, objBox);
}

bool PhysicsScene::Box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);

	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;
		box1->CheckBoxCorners(*box2, contact, numContacts, pen, norm);
		if (box2->CheckBoxCorners(*box1, contact, numContacts, pen, norm))
		{
			norm = -norm;
		}
		if (pen > 0)
		{
			box1->ResolveCollision(box2, contact / float(numContacts), &norm, pen);
		}
		return true;
	}
	return false;
}

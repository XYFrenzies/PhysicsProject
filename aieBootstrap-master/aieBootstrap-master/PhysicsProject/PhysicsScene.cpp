#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include "Sphere.h"
#include "Plane.h"
#include <list>
#include <iostream>

//Function pointer array for handing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn colFuncArray[] =
{
	PhysicsScene::Plane2Plane,  PhysicsScene::Plane2Sphere,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere,
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
	int actorCount = m_actors.size();
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* objOuter = m_actors[outer];
			PhysicsObject* objInner = m_actors[inner];
			int shapeID_out = objOuter->GetShapeID();
			int shapeID_in = objInner->GetShapeID();
			
			int funcIndex = (shapeID_out * SHAPE_COUNT) + shapeID_in;
			fn colFuncPtr = colFuncArray[funcIndex];
			if (colFuncPtr != nullptr)
			{
				colFuncPtr(objOuter, objInner);
			}
		}
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

bool PhysicsScene::Sphere2Plane(PhysicsObject* objSphere, PhysicsObject* objPlane)
{
	Sphere* sphere = dynamic_cast<Sphere*>(objSphere);
	Plane* plane = dynamic_cast<Plane*>(objPlane);


	//If successful and they both have a value.
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 colNorm = plane->GetNormal();
		float sphereToPlane = glm::dot(sphere->GetPosition(), 
			colNorm - plane->GetDistance());
		float intersec = sphere->GetRadius() - sphereToPlane;//Interaction 
		float velOutOfPlane = glm::dot(sphere->GetVelocity(), colNorm);
		if (intersec > 0 && velOutOfPlane < 0)
		{
			glm::vec2 holdVel = sphere->GetVelocity();
			sphere->ApplyForce(-holdVel * sphere->GetMass()
				+ glm::vec2(holdVel.x * 4, -holdVel.y * 4));//Gives the opposite effect of what the object will have.
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
			sphere1->ResolveCollision(sphere2);
			return true;
		}
	}
	return false;
}

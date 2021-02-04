#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Rigidbody.h"

#include <list>
#include <iostream>
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

		for (auto pActor : m_actors)
		{
			for (auto pOther : m_actors)
			{
				if (pActor == pOther)
					continue;

				if (std::find(objs.begin(), objs.end(), pActor) != objs.end() &&
					std::find(objs.begin(), objs.end(), pOther) != objs.end())
					continue;

				Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
				if (pRigid->CheckCollision(pOther) == true)
				{
					pRigid->ApplyForceToOther(dynamic_cast<Rigidbody*>(pOther),
						pRigid->GetVelocity()* pRigid->GetMass());
					objs.push_back(pRigid);
					objs.push_back(pOther);
				}
			}
		}
		objs.clear();
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

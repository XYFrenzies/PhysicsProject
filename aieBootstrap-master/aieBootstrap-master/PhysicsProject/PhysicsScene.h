#pragma once
#include <glm/glm.hpp>
#include <vector>
class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();//Constructor
	~PhysicsScene();//Deconstructor

	//Adds a Physics object
	void AddActor(PhysicsObject* a_actor);
	//Removes a Physics Object
	void RemoveActor(PhysicsObject* a_actor);

	//Updates per frame due to the delta time, the update will be called in the PhysicsObject class.
	//This is dealing with the collision detection and resolution
	void Update(float dt);

	//Called once per frame to handle rendering of Physics Objects.
	//Will be added to a loop of Gizmo objects to render.
	void Draw();
	void DebugScene();

	void SetGravity(const glm::vec2 a_gravity) { m_gravity = a_gravity; }
	glm::vec2 GetGravity() const { return m_gravity; }

	void SetTimeStep(const float a_timeStep) { m_timeStep = a_timeStep; }
	float GetTimeStep() const { return m_timeStep; }

	void CheckCollisions();

	static bool Plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool Plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Plane2Box(PhysicsObject*, PhysicsObject*);

	static bool Sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Box(PhysicsObject*, PhysicsObject*);

	static bool Box2Plane(PhysicsObject*, PhysicsObject*);
	static bool Box2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Box2Box(PhysicsObject*, PhysicsObject*);
	


private:
protected:
	//Gravity for both x and y axis
	glm::vec2 m_gravity;
	float m_timeStep;

	std::vector<PhysicsObject*> m_actors;

};


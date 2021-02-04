#pragma once
#include <glm/glm.hpp>


//This is a abstract class. It will be used as a base for the 
//derivated classes that will use FixedUpdate
enum ShapeType
{
	PLANE = 0,
	SPHERE,
	BOX
};

class PhysicsObject
{
public:
	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;
	virtual void ResetPosition() {};
private:
protected:
	ShapeType m_shapeID;
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

};


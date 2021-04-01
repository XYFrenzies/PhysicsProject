#pragma once
#include <glm/glm.hpp>
//This file contains the camera's transform in different perspectives and its movement in world space.
//Created by Benjamin McDonald on the 11th March 2021
//Last accessed: 1st April 2021
class Camera
{
public:
	Camera();
	~Camera() {};
	void Update(float a_deltaTime);
	glm::vec3 GetPosition() { return m_position; }
	glm::vec3 SetPosition(glm::vec3 a_position) 
	{ return m_position = a_position; }
	void SetStaticCam() { isCameraStatic = true; }

	void SetPerspective(float a_fieldOfView, float a_aspectRatio,
		float a_near, float a_far);//Creates a perspective of the camera in 3D space.
	void SetLookAt(glm::vec3 a_from, glm::vec3 a_to,
		glm::vec3 a_up);//Creates a viewpoint of the camera to 3D space.

	glm::mat4 GetViewMatrix();//Returns the view matrix.
	glm::mat4 GetProjectionMatrix();//Returns the projection matrix

	glm::mat4 GetWorldTransform();//Returns the world transform matrix

	glm::mat4 GetProjectionViewMatrix() //Gets the multiplication of the view and projection matrix
	{ 
		return m_projectionViewTranform = 
		m_projectionTransform * m_viewTransform; 
	};
	float GetTheta() { return m_theta; }//Gets the value of theta
	float GetPhi() { return m_phi; }//Gets the value of Phi
private:
	float m_theta; //In degrees
	float m_phi;   //In degrees
	glm::vec3 m_position;
	float m_lastMouseX,
		  m_lastMouseY;
	float cameraSpeed = 5.0f;
	float cameraRotSpeed = 3.0f;//Rotation speed
	bool isCameraStatic = false;
	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_projectionViewTranform;
protected:
};


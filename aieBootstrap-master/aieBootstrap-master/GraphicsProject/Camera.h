#pragma once
#include <glm/glm.hpp>
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
		float a_near, float a_far);
	void SetLookAt(glm::vec3 a_from, glm::vec3 a_to,
		glm::vec3 a_up);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	glm::mat4 GetWorldTransform();

	glm::mat4 GetProjectionViewMatrix() 
	{ 
		return m_projectionViewTranform = 
		m_projectionTransform * m_viewTransform; 
	};
	float GetTheta() { return m_theta; }
	float GetPhi() { return m_phi; }
private:
	float m_theta; //In degrees
	float m_phi;   //In degrees
	glm::vec3 m_position;
	float m_lastMouseX,
		  m_lastMouseY;
	float cameraSpeed = 5.0f;
	float cameraRotSpeed = 3.0f;
	bool isCameraStatic = false;
	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_projectionViewTranform;
protected:
};


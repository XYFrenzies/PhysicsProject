#include "Camera.h"
#include "glm/ext.hpp"
#include <Input.h>
Camera::Camera()
{
	m_phi = 0;
	m_theta = 0;
	m_position = glm::vec3(-10, 2, 0);
}
void Camera::Update(float a_deltaTime)
{
	if (!isCameraStatic)
	{
		aie::Input* input = aie::Input::getInstance();
		float thetaR = glm::radians(m_theta);
		float phiR = glm::radians(m_phi);
		//Calculate the forwards and right axis
		// and the up axis for the camera
		glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR),
			glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));
		glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));
		glm::vec3 up(0, 1, 0);
		//This is to get the inputs per update for the movement of the camera.

#pragma region InputSettings
		if (input->isKeyDown(aie::INPUT_KEY_W) && input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
			m_position += forward * a_deltaTime * cameraSpeed;
		else if (input->isKeyDown(aie::INPUT_KEY_W))
			m_position += forward * a_deltaTime;
		if (input->isKeyDown(aie::INPUT_KEY_A) && input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
			m_position -= right * a_deltaTime * cameraSpeed;
		else if (input->isKeyDown(aie::INPUT_KEY_A))
			m_position -= right * a_deltaTime;
		if (input->isKeyDown(aie::INPUT_KEY_S) && input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
			m_position -= forward * a_deltaTime * cameraSpeed;
		else if (input->isKeyDown(aie::INPUT_KEY_S))
			m_position -= forward * a_deltaTime;
		if (input->isKeyDown(aie::INPUT_KEY_D) && input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
			m_position += right * a_deltaTime * cameraSpeed;
		else if (input->isKeyDown(aie::INPUT_KEY_D))
			m_position += right * a_deltaTime;
		if (input->isKeyDown(aie::INPUT_KEY_Z))
			m_position -= up * a_deltaTime;
		if (input->isKeyDown(aie::INPUT_KEY_X))
			m_position += up * a_deltaTime;
#pragma endregion


		//Gert mouse positions 
		float mX = input->getMouseX();
		float mY = input->getMouseY();
		const float turnSpeed = glm::radians(180.0f);
		//IF thge right mouse buitton is down, increment the theta anbd phi
		if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
		{
			m_theta += turnSpeed * (mX - m_lastMouseX) * a_deltaTime * cameraRotSpeed;
			m_phi += turnSpeed * (mY - m_lastMouseY) * a_deltaTime * cameraRotSpeed;
		}
		//Now store the frames last values for the next
		m_lastMouseX = mX;
		m_lastMouseY = mY;
	}
}

glm::mat4 Camera::GetViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR),
		glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));
	if (!isCameraStatic)
		return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
	else
		return glm::lookAt(m_position, glm::vec3(0) + forward, glm::vec3(0, 2, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float a_width, float a_height)
{
	return glm::perspective(glm::pi<float>() * 0.25f,
		a_width / a_height, 0.1f, 1000.0f);
}


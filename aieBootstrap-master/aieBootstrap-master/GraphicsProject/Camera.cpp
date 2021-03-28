#include "Camera.h"
#include "glm/ext.hpp"
#include <Input.h>
Camera::Camera() : m_lastMouseX(), m_lastMouseY(), m_projectionTransform(),
m_projectionViewTranform(), m_viewTransform(), m_worldTransform()
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
		//Within these inputs, it is determined whether the player is holding the shift key or not.
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
		constexpr float turnSpeed = glm::radians(180.0f);
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
/// <summary>
/// Creates a perspective for the camera.
/// </summary>
/// <param name="a_fieldOfView"></param>
/// <param name="a_aspectRatio"></param>
/// <param name="a_near"></param>
/// <param name="a_far"></param>
void Camera::SetPerspective(float a_fieldOfView, float a_aspectRatio, float a_near, float a_far)
{
	m_projectionTransform = glm::perspective(a_fieldOfView, 
		a_aspectRatio, a_near, a_far);
}
/// <summary>
/// Creates a look at functrion for the camera to start looking at the position in view space.
/// </summary>
/// <param name="a_from"></param>
/// <param name="a_to"></param>
/// <param name="a_up"></param>
void Camera::SetLookAt(glm::vec3 a_from, glm::vec3 a_to, glm::vec3 a_up)
{
	m_viewTransform = glm::lookAt(a_from, a_to, a_up);
}
/// <summary>
/// Gets the view matrix of the camera. This is determined by the movement in the z axis and the rotatgion of the camera.
/// It returns a specific vazlue depending on if the camera is static or not.
/// </summary>
/// <returns></returns>
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
/// <summary>
/// Returns the projection matrix that is predefined.
/// </summary>
/// <returns></returns>
glm::mat4 Camera::GetProjectionMatrix()
{
	return m_projectionTransform;
}
/// <summary>
/// Gets the world transform of the camera. Returns the translation, rotation and scale of the camera.
/// </summary>
/// <returns></returns>
glm::mat4 Camera::GetWorldTransform()
{

	glm::mat4& translation = glm::translate(glm::mat4(1), GetPosition());
	glm::mat4& rotation = 
		glm::rotate(glm::mat4(1), GetTheta(), glm::vec3(1, 0, 0)) * 
		glm::rotate(glm::mat4(1), GetPhi(), glm::vec3(0, 1, 0)) * 
		glm::rotate(glm::mat4(1), GetTheta(), glm::vec3(0, 0, 1));
	glm::mat4& scale = glm::scale(glm::mat4(1), glm::vec3(1));


	return translation * rotation * scale;
}


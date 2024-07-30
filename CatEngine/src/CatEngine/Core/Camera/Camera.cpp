#include "cepch.h"
#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"


namespace CatEngine {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float Near, float Far)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, Near, Far)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float Near, float Far)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, Near, Far);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
#pragma once

#include "glm/glm.hpp"

namespace CatEngine 
{

	class OrthographicCamera 
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float Near, float Far);


		const glm::vec3 GetPosition() const { return m_Position; }
		const glm::vec3 GetRotation() const  { return m_Rotation; }

		void SetPosition(const glm::vec3& position) 
		{
			m_Position = position;
			RecalculateViewMatrix();
		}
		void SetRotation(const glm::vec3& rotation) 
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}

		const glm::mat4& GetProjectionMatrix() const        { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const              { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const    { return m_ViewProjectionMatrix; }

		float m_Pos[2] = { 0 };

	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

	};



}
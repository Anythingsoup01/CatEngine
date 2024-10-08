#pragma once

#include "glm/glm.hpp"

namespace CatEngine 
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.f);
	};



	class OrthographicCamera 
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float Near = -1.f, float Far = 1.f);

		void SetProjection(float left, float right, float bottom, float top, float Near = -1.f, float Far = 1.f);

		const glm::vec3 GetPosition() const { return m_Position; }
		const float GetRotation() const  { return m_Rotation; }

		void SetPosition(const glm::vec3& position) 
		{
			m_Position = position;
			RecalculateViewMatrix();
		}
		void SetRotation(const float rotation) 
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}


		const glm::mat4& GetProjectionMatrix() const        { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const              { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const    { return m_ViewProjectionMatrix; }


	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.f,0.f,0.f };
		float m_Rotation = 0.f;
	};



}
#pragma once

#include "Camera.h"

#include "CatEngine/Core/TimeStep.h"

#include "CatEngine/Events/ApplicationEvent.h"
#include "CatEngine/Events/MouseEvent.h"
#include "CatEngine/Events/KeyEvent.h"

#include <glm/glm.hpp>

namespace CatEngine
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio);

		void OnUpdate(Time deltaTime);
		void OnEvent(Event& e);

		void OnResizeBounds(float width, float height);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float zoomLevel);

	private:
		void RecalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		bool m_Panning = false;

		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		float m_ZoomSpeed = 0.1f;
		float m_CameraRotation = 0.f;
		float m_CameraRotationSpeed = 2.0f;
		float m_CameraSpeed = 1.f;

		glm::vec3 m_CameraPosition = { 0, 0, 0 };

		OrthographicCamera m_Camera;

	};

}
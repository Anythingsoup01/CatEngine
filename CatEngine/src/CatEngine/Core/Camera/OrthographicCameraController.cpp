#include "cepch.h"
#include "OrthographicCameraController.h"

#include "CatEngine/Core/Input.h"

#include "CatEngine/Core/MouseButtonCodes.h"
#include "CatEngine/Core/KeyCodes.h"

namespace CatEngine
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}
	void OrthographicCameraController::OnUpdate(Time time)
	{
		CE_PROFILE_FUNCTION();
		// Camera Up
		m_CameraPosition.y += (Input::IsKeyPressed(CE_W)) ? m_CameraSpeed * time.deltaTime() : 0;
		// Camera Down
		m_CameraPosition.y -= (Input::IsKeyPressed(CE_S)) ? m_CameraSpeed * time.deltaTime() : 0;
		// Camera Right
		m_CameraPosition.x += (Input::IsKeyPressed(CE_D)) ? m_CameraSpeed * time.deltaTime() : 0;
		// Camera Left
		m_CameraPosition.x -= (Input::IsKeyPressed(CE_A)) ? m_CameraSpeed * time.deltaTime() : 0;
		// Camera Rotate Right
		m_CameraRotation += Input::IsKeyPressed(CE_E) ? m_CameraRotationSpeed * time.deltaTime() : 0;
		// Camera Rotate Left
		m_CameraRotation -= Input::IsKeyPressed(CE_Q) ? m_CameraRotationSpeed * time.deltaTime() : 0;
		// Camera Rotate Reset
		m_CameraRotation = Input::IsKeyPressed(CE_R) ? 0 : m_CameraRotation;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}
	void OrthographicCameraController::SetZoomLevel(float zoomLevel)
	{
		m_ZoomLevel = zoomLevel;
		RecalculateView();
	}
	void OrthographicCameraController::RecalculateView()
	{
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		CE_PROFILE_FUNCTION();
		m_ZoomLevel -= e.GetYOffset() * m_ZoomSpeed;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);
		
		RecalculateView();

		m_CameraSpeed = m_ZoomLevel;
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		CE_PROFILE_FUNCTION();
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		RecalculateView();
		return false;
	}
}
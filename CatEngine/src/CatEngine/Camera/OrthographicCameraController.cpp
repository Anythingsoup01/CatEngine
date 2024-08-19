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
	bool IsShiftPressed()
	{
		return (Input::IsKeyPressed(CE_LEFT_SHIFT) || Input::IsKeyPressed(CE_RIGHT_SHIFT));
	}
	void OrthographicCameraController::OnUpdate(Time time)
	{
		CE_PROFILE_FUNCTION();


		if ((IsShiftPressed() && Input::IsMouseButtonPressed(CE_MOUSE_BUTTON_RIGHT)) || m_Panning)
		{
			m_Panning = true;
			m_CameraPosition += glm::vec3((Input::GetMouseXOffset() * m_CameraSpeed) * time.deltaTime(), (Input::GetMouseYOffset() * -m_CameraSpeed) * time.deltaTime(), 0);
		}
		if (Input::IsMouseButtonReleased(CE_MOUSE_BUTTON_RIGHT))
		{
			m_Panning = false;
			Input::ResetMouseOffset();
		}
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}
	void OrthographicCameraController::OnResizeBounds(float width, float height)
	{
		m_AspectRatio = width / height;
		RecalculateView();
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

		m_CameraSpeed = m_ZoomLevel / 2.f;
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		CE_PROFILE_FUNCTION();
		OnResizeBounds((float)e.GetWidth(), (float)e.GetHeight());
		RecalculateView();
		return false;
	}
}
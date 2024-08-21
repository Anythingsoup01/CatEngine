#pragma once
#include "CatEngine.h"

namespace CatEngine
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Time time) override;
		virtual void OnImGuiDraw() override;
		virtual void OnEvent(Event& e) override;

		bool OnWindowResize(WindowResizeEvent& e);

	private:

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		bool m_Minimized = false;
		
		Ref<Scene> m_ActiveScene;

		Entity m_SquareEntity;

		Entity m_CameraEntity;

		Ref<Texture2D> m_Texture;

		glm::vec4 m_SquareColor{ 1.f,1.f,1.f,1.f };

		OrthographicCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = { 0.f,0.f };

	};
}
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

		virtual void OnUpdate(CatEngine::Time time) override;
		virtual void OnImGuiDraw() override;
		virtual void OnEvent(CatEngine::Event& e) override;
	private:

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec3 m_Transform = { 0,0,0 };
		glm::vec3 m_Rotation = { 0,0,0 };
		glm::vec3 m_Scale = { 0,0,0 };

		OrthographicCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = { 0.f,0.f };

	};
}
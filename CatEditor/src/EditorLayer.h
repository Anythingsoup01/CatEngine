#pragma once
#include "CatEngine.h"
#include "Panels/SceneHierarchyPanel.h"

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
	private:

		void ImGuizmoDraw(Entity selectedEntity,const glm::mat4& cameraProjection, glm::mat4 cameraView);

		bool OnWindowResize(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

		void SaveSceneAs();
		void SaveScene();
		void OpenScene();
		void NewScene();

	private:

		std::string m_SceneFilePath;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		bool m_Minimized = false;
		bool m_RuntimeActive = false;
		bool m_CameraPreviewActive = false;
		

		EditorCamera m_EditorCamera;
		Ref<Scene> m_ActiveScene;

		OrthographicCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer;

		int m_GizmoType = -1;

		glm::vec2 m_ViewportSize = { 0.f,0.f };
		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}
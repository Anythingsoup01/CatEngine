#pragma once
#include "CatEngine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

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
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void SaveSceneAs();
		void SaveScene();

		void OpenScene();
		void OpenScene(const std::filesystem::path& filePath);
		
		void NewScene();
		
		void OnScenePlay();
		void OnScenePause();
		void OnSceneStop();

		void OnSceneSimulateStart();
		void OnSceneSimulateStop();
		
		// Key-bound funcitons

		void DuplicateEntity();
		void DeleteEntity();
		void CopyEntity();
		void PasteEntity();




		// TODO: Implement pausing and simulating physics

		void OnOverlayRender();


		// UI Panels
		void UI_Toolbar();

	private:

		std::filesystem::path m_SceneFilePath;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		bool m_Minimized = false;
		bool m_BlockEvents = false;

		bool m_IsScenePaused = false;

		enum class SceneState
		{
			Edit = 0,
			Play = 1,
			Simulate = 2
		};
		SceneState m_SceneState = SceneState::Edit;
		
		Entity m_HoveredEntity;
		Entity m_CopiedEntity;

		EditorCamera m_EditorCamera;
		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;

		Ref<Framebuffer> m_Framebuffer;

		// Editor Resources
		Ref<Texture2D> m_IconStartRuntime, m_IconPauseRuntime, m_IconPauseRuntimeSelected, m_IconNextFrameRuntime, m_IconStopRuntime;
		Ref<Texture2D> m_IconStartSimulation;

		int m_GizmoType = -1;

		glm::vec2 m_ViewportSize = { 0.f,0.f };
		glm::vec2 m_ViewportBounds[2];
		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;


	};
}

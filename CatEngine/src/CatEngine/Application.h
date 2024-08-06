#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/Events.h"

#include "ImGui/ImGuiLayer.h"

#include "Core/Core.h"
#include "Core/Layers/LayerStack.h"
#include "Core/Renderer/Renderer.h"
#include "Core/TimeStep.h"
#include "Core/Window.h"


namespace CatEngine {
	class Application {
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }

		void CloseEditor();

	private:
		LayerStack m_LayerStack;
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		float m_LastFrameTime = 0.f;
	private:
		static Application* s_Instance;
		bool m_Running = true;
		bool m_Minimized = false;
	};

	// Defined in client
	Application* CreateApplication();
}
#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/Events.h"

#include "ImGui/ImGuiLayer.h"

#include "Core/Core.h"
#include "Core/TimeStep.h"
#include "Core/Window.h"

#include "Layers/LayerStack.h"
#include "Renderer/Renderer.h"


namespace CatEngine {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			API_ASSERT(index < Count, "");
			return Args[index];
		}
	};
	class Application {


	public:
		Application(const std::string& name = "CatEngine", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }

		void CloseEditor();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		bool m_Minimized = false;
	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
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
	};

	// Defined in client
	Application* CreateApplication(ApplicationCommandLineArgs args);
}
#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/Events.h"

#include "ImGui/ImGuiLayer.h"

#include "Core/Core.h"
#include "Core/TimeStep.h"
#include "Core/Window.h"

#include "Layers/LayerStack.h"
#include "Renderer/Renderer.h"

#include "Scripting/ScriptEngine.h"


namespace CatEngine {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			CE_ASSERT(index < Count);
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "CatEngine";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandlineArgs;
	};

	class Application {
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }

		void CloseEditor();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		const ApplicationSpecification GetSpecification() const { return m_Specification; }

	private:
		ApplicationSpecification m_Specification;
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
	Application* CreateApplication(ApplicationCommandLineArgs args);
}

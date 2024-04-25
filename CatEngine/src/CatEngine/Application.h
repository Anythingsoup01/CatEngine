#pragma once

#include "Window.h"
#include "CatEngine/Core/Layers/LayerStack.h"
#include "Events/Events.h"
#include "Events/ApplicationEvent.h"


namespace CatEngine {
	class Application {
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Events& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// Defined in client
	Application* CreateApplication();
}
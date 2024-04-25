#pragma once


#include "Events/Events.h"
#include "Events/ApplicationEvent.h"

#include "Window.h"

namespace CatEngine {
	class Application {
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Events& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// Defined in client
	Application* CreateApplication();
}
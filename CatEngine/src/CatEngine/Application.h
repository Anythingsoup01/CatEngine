#pragma once

#include "Core/Core.h"

#include "Events/EventHandler.h"
#include "Events/ApplicationEvent.h"

#include "Window.h"



namespace CatEngine {

	class __declspec(dllexport) Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(EventHandler& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	//Defined in client
	Application* CreateApplication();
}

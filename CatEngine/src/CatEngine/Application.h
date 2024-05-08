#pragma once

#include "Core/Window.h"
#include "CatEngine/Core/Layers/LayerStack.h"
#include "Events/Events.h"
#include "Events/ApplicationEvent.h"


#include "Core/Logging/Log.h"
#include "Core/Core.h"
#include "Core/Input/Input.h"


namespace CatEngine {
	class Application {
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Events& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
	};

	// Defined in client
	Application* CreateApplication();
}
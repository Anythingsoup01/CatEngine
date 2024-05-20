#pragma once

#include "Core/Window.h"
#include "CatEngine/Core/Layers/LayerStack.h"
#include "Events/Events.h"
#include "Events/ApplicationEvent.h"

#include "CatEngine/ImGui/ImGuiLayer.h"
 
#include "CatEngine/Core/Renderer/Buffers/Buffer.h"
#include "CatEngine/Core/Renderer/Buffers/VertexArray.h"
#include "CatEngine/Core/Renderer/Rendering/Renderer.h"
#include "CatEngine/Core/Renderer/Shader/OpenGLShader.h"

namespace CatEngine {
	class Application {
	public:
		Application();
		virtual ~Application();

		virtual void Run() = 0;
		void OnEvent(Events& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		std::shared_ptr<OpenGLShader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;


		bool m_Running = true;
		static Application* s_Instance;
	private:
		bool OnWindowClose(WindowCloseEvent& e);
	};

	// Defined in client
	Application* CreateApplication();
}
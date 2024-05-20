#include "cepch.h"
#include "Application.h"

#include "Core/Logging/Log.h"
#include "Core/Core.h"
#include "Core/Input/Input.h"


namespace CatEngine {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{

		/*s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		// Vertex Array
		m_VertexArray.reset(VertexArray::Create());

		// Vertex Buffer
		float vertices[7 * 4]{
			-0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f, // 0
			 0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f, // 1
			-0.5f,  0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f, // 2
			 0.5f,  0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f, // 2
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout
		({
			{ ShaderDataType::Vec3, "a_Position", false },
			{ ShaderDataType::Vec4, "a_Color", false }
		});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer

		unsigned int indices[]{ 0, 1, 2, 1, 3, 2};

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Shader / Texture Binding

		std::string vertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			out vec3 v_Position;
			out vec4 v_Color;	

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
			    gl_Position = vec4(a_Position, 1.0);
			}; 
		)";
		std::string fragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
			    color =  vec4(v_Position * 0.5 + 0.5, 1.0);
			    color =  v_Color;
			};
		)";

		m_Shader.reset(new OpenGLShader(vertexSource, fragmentSource));*/

	}
	Application::~Application()
	{
	}

	//void Application::Run()
	//{
	//	while (m_Running) {
	//		RenderCommand::Clear({ 0.1, 0.1, 0.1, 1.0 });
	//	
	//		Renderer::BeginScene();
	//		m_Shader->Bind();
	//		Renderer::Submit(m_VertexArray);
	//		Renderer::EndScene();
	//	
	//		RenderCommand::DrawIndexed(m_VertexArray);
	//	
	//		for (Layer* layer : m_LayerStack)
	//			layer->OnUpdate();
	//	
	//		m_ImGuiLayer->Begin();
	//		for (Layer* layer : m_LayerStack)
	//			layer->OnImGuiDraw();
	//		m_ImGuiLayer->End();
	//	
	//		m_Window->OnUpdate();
	//	}
	//}

	void Application::OnEvent(Events& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}
#include <iostream>

#include "CatEngine.h"

#include "imgui.h"

class ExampleLayer : public CatEngine::Layer {
public:
	ExampleLayer()
		:Layer("Example") {}
	
	void OnUpdate() override 
	{
		//CLI_INFO("ExampleLayer::Update");
	}

	void OnEvent(CatEngine::Events& event) override 
	{
		if (event.GetEventType() == CatEngine::EventType::KeyPressed) {
			CatEngine::KeyPressedEvent& e = (CatEngine::KeyPressedEvent&)event;
			API_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public CatEngine::Application {
public:
	Sandbox() {
		s_Instance = this;
		PushLayer(new ExampleLayer());

		m_Window = std::unique_ptr<CatEngine::Window>(CatEngine::Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new CatEngine::ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		// Vertex Array
		m_VertexArray.reset(CatEngine::VertexArray::Create());

		// Vertex Buffer
		float vertices[7 * 4]{
			-0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f, // 0
			 0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f, // 1
			-0.5f,  0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f, // 2
			 0.5f,  0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f, // 2
		};

		std::shared_ptr<CatEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(CatEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout
		({
			{ CatEngine::ShaderDataType::Vec3, "a_Position", false },
			{ CatEngine::ShaderDataType::Vec4, "a_Color", false }
			});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer

		unsigned int indices[]{ 0, 1, 2, 1, 3, 2 };

		std::shared_ptr<CatEngine::IndexBuffer> indexBuffer;
		indexBuffer.reset(CatEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

		m_Shader.reset(new CatEngine::OpenGLShader(vertexSource, fragmentSource));

	}
	~Sandbox() {

	}
	void Run() {
		while (m_Running) {
			CatEngine::RenderCommand::Clear({ 0.1, 0.1, 0.1, 1.0 });

			CatEngine::Renderer::BeginScene();
			m_Shader->Bind();
			CatEngine::Renderer::Submit(m_VertexArray);
			CatEngine::Renderer::EndScene();

			CatEngine::RenderCommand::DrawIndexed(m_VertexArray);

			for (CatEngine::Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (CatEngine::Layer* layer : m_LayerStack)
				layer->OnImGuiDraw();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
};

CatEngine::Application* CatEngine::CreateApplication() {
	return new Sandbox();
}
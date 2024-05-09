#include "cepch.h"
#include "Application.h"
#include <glad/glad.h>

#include <glm/glm.hpp>

#include "Core/Logging/Log.h"
#include "Core/Core.h"
#include "Core/Input/Input.h"


namespace CatEngine {

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case CatEngine::ShaderDataType::Vec:  return GL_FLOAT;
		case CatEngine::ShaderDataType::Vec2: return GL_FLOAT;
		case CatEngine::ShaderDataType::Vec3: return GL_FLOAT;
		case CatEngine::ShaderDataType::Vec4: return GL_FLOAT;
		case CatEngine::ShaderDataType::Mat3: return GL_FLOAT;
		case CatEngine::ShaderDataType::Mat4: return GL_FLOAT;
		case CatEngine::ShaderDataType::Int:  return GL_INT;
		case CatEngine::ShaderDataType::Int2: return GL_INT;
		case CatEngine::ShaderDataType::Int3: return GL_INT;
		case CatEngine::ShaderDataType::Int4: return GL_INT;
		case CatEngine::ShaderDataType::Bool: return GL_BOOL;
		}
		API_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	Application::Application()
	{

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		float vertices[7 * 4]{
			-0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f, // 0
			 0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f, // 1
			-0.5f,  0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f, // 2
			 0.5f,  0.5f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f, // 3
		};

		unsigned int indices[]{
			0, 1, 2,
			1, 3, 2
		};

		// Vertex Array
		glCreateVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);
		// Vertex Buffer

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		{
			BufferLayout layout = {
				{ ShaderDataType::Vec3, "a_Position", false },
				{ ShaderDataType::Vec4, "a_Color", false }
			};

			m_VertexBuffer->SetLayout(layout);
		}


		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index, 
				element.GetComponentCount(element.Type), 
				ShaderDataTypeToGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		// Index Buffer
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

		m_Shader.reset(new OpenGLShader(vertexSource, fragmentSource));
	}
	Application::~Application()
	{
		glDeleteVertexArrays(1, &m_VertexArray);
	}
	void Application::Run()
	{
		while (m_Running) {
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiDraw();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();




		}
	}

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
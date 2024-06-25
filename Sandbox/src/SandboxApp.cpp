#include <iostream>

#include "CatEngine.h"

#include "imgui.h"

class AppLayer : public CatEngine::Layer {
public:
	AppLayer()
		:Layer("Application"), m_Camera(-1.77, 1.77, -1.0, 1.0, -1.0, 1.0) 
	{
		// Vertex Array
		m_VertexArray.reset(CatEngine::VertexArray::Create());

		// Vertex Buffer
		float vertices[7 * 4]{
			-0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f, // 0
			 0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f, // 1
			-0.5f,  0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f, // 2
			 0.5f,  0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f, // 3
		};

		std::shared_ptr<CatEngine::VertexBuffer> squareBuffer;
		squareBuffer.reset(CatEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		squareBuffer->SetLayout
		({
			{ CatEngine::ShaderDataType::Vec3, "a_Position", false },
			{ CatEngine::ShaderDataType::Vec4, "a_Color", false }
			});
		m_VertexArray->AddVertexBuffer(squareBuffer);


		// Index Buffer

		unsigned int indices[]{ 0, 1, 2, 1, 3, 2 };

		std::shared_ptr<CatEngine::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(CatEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(squareIndexBuffer);



		m_TriangleVA.reset(CatEngine::VertexArray::Create());

		float triangleVerts[7 * 4]{
			 1.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f, // 0
			 0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f, // 1
			 0.5f,  0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f, // 2
			 1.5f,  0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f, // 3
		};

		std::shared_ptr<CatEngine::VertexBuffer> triangleVertexBuffer;
		triangleVertexBuffer.reset(CatEngine::VertexBuffer::Create(triangleVerts, sizeof(triangleVerts)));

		triangleVertexBuffer->SetLayout
		({
			{ CatEngine::ShaderDataType::Vec3, "a_Position", false },
			{ CatEngine::ShaderDataType::Vec4, "a_Color", false }
			});
		m_TriangleVA->AddVertexBuffer(triangleVertexBuffer);

		unsigned int triangleIndices[]{ 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<CatEngine::IndexBuffer> triangleIndexBuffer;
		triangleIndexBuffer.reset(CatEngine::IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t)));
		m_TriangleVA->SetIndexBuffer(triangleIndexBuffer);

		// Shader / Texture Binding

		std::string vertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;	

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
			    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader.reset(new CatEngine::Shader(vertexSource, fragmentSource));

		std::string triVertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;	

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
			    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}; 
		)";
		std::string triFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
			    color =  vec4(v_Position * 0.5 + 0.5, 1.0);
			};
		)";

		m_TriangleShader.reset(new CatEngine::Shader(triVertexSource, triFragmentSource));
	}
	
	void OnUpdate(CatEngine::Time time) override 
	{	
		CLI_INFO("DeltaTime: {0}s, ({1}ms)", time.deltaTime(), time.deltaTimeMS());


		CatEngine::RenderCommand::Clear({ 0.1, 0.1, 0.1, 1.0 });

		CatEngine::Renderer::BeginScene(m_Camera);

		CatEngine::Renderer::Submit(m_Shader, m_VertexArray);
		CatEngine::Renderer::Submit(m_TriangleShader, m_TriangleVA);

		CatEngine::Renderer::EndScene();

		m_CameraPosition.y += (CatEngine::Input::IsKeyPressed(CE_W) || CatEngine::Input::IsKeyPressed(CE_UP)) ? m_CameraSpeed * time.deltaTime() : 0;
		m_CameraPosition.y -= (CatEngine::Input::IsKeyPressed(CE_S) || CatEngine::Input::IsKeyPressed(CE_DOWN)) ? m_CameraSpeed * time.deltaTime() : 0;
		m_CameraPosition.x += (CatEngine::Input::IsKeyPressed(CE_D) || CatEngine::Input::IsKeyPressed(CE_RIGHT)) ? m_CameraSpeed * time.deltaTime() : 0;
		m_CameraPosition.x -= (CatEngine::Input::IsKeyPressed(CE_A) || CatEngine::Input::IsKeyPressed(CE_LEFT)) ? m_CameraSpeed * time.deltaTime() : 0;

		m_CameraRotation += CatEngine::Input::IsKeyPressed(CE_E) ? m_CameraSpeed * time.deltaTime() : 0;
		m_CameraRotation -= CatEngine::Input::IsKeyPressed(CE_Q) ? m_CameraSpeed * time.deltaTime() : 0;

		m_Camera.SetPosition({ m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z });
		m_Camera.SetRotation({ 0, 0, m_CameraRotation });

	}

	void OnEvent(CatEngine::Events& event) override 
	{

	}
private:

	std::shared_ptr<CatEngine::Shader> m_Shader;
	std::shared_ptr<CatEngine::Shader> m_TriangleShader;
	std::shared_ptr<CatEngine::VertexArray> m_VertexArray;
	std::shared_ptr<CatEngine::VertexArray> m_TriangleVA;

	float m_CameraSpeed = 1.0f;

	glm::vec3 m_CameraPosition = { 0, 0, 0 };

	float m_CameraRotation = 0;


	CatEngine::OrthographicCamera m_Camera;
};


class Sandbox : public CatEngine::Application {
public:
	Sandbox()
	{
		PushLayer(new AppLayer());
	}
	~Sandbox() {

	}
};

CatEngine::Application* CatEngine::CreateApplication() {
	return new Sandbox();
}
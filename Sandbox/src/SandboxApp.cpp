#include <iostream>

#include "CatEngine.h"

#include "Platform/OpenGL/Renderer/OpenGLShader.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class AppLayer : public CatEngine::Layer {
public:
	AppLayer()
		: Layer("Application"), m_CameraController(1280.f / 720.f)
	{
		// Vertex Array
		m_SquareVertexArray.reset(CatEngine::VertexArray::Create());

		// Vertex Buffer
		float vertices[7 * 4]{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.3f, 1.0f, // 0
			-0.5f,  0.5f, 0.0f, 0.3f, 0.2f, 0.8f, 1.0f, // 1
			 0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.3f, 1.0f, // 2
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 3
		};

		CatEngine::Ref<CatEngine::VertexBuffer> squareBuffer;
		squareBuffer.reset(CatEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		squareBuffer->SetLayout
		({
			{ CatEngine::ShaderDataType::Vec3, "a_Position", false },
			{ CatEngine::ShaderDataType::Vec4, "a_Color", false},
			});
		m_SquareVertexArray->AddVertexBuffer(squareBuffer);

		// Index Buffer
		unsigned int indices[]{ 0, 1, 2, 1, 3, 2 };

		CatEngine::Ref<CatEngine::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(CatEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		auto flatColorShader = m_ShaderLibrary.Load("FlatColorShader", "assets/shaders/FlatShader.glsl");
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		auto name = flatColorShader.get()->GetName();
		
		m_Texture2D = CatEngine::Texture2D::Create("assets/textures/cat_texture.png");
		std::dynamic_pointer_cast<CatEngine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<CatEngine::OpenGLShader>(textureShader)->UploadUniformInt("u_Color", 0);
	}
	
	void OnUpdate(CatEngine::Time time) override 
	{
		m_CameraController.OnUpdate(time);
		CatEngine::RenderCommand::Clear({ 0.1, 0.1, 0.1, 1.0 });

		CatEngine::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto flatColorShader = m_ShaderLibrary.Get("FlatColorShader");
		auto textureShader = m_ShaderLibrary.Get("Texture");


		flatColorShader->Bind();
		m_Texture2D->Bind();

		if (renderSquare)
		{
			for (int x = 0; x < m_GridSize[0]; x++)
			{
				for (int y = 0; y < m_GridSize[1]; y++)
				{
					glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 squareOneTransform = glm::translate(glm::mat4(1.0f), pos + m_SquareTransform) * scale;
					CatEngine::Renderer::Submit(flatColorShader, m_SquareVertexArray, squareOneTransform);
				}
			}
		}
		if (renderBigCat)
			CatEngine::Renderer::Submit(flatColorShader, m_SquareVertexArray, glm::mat4(glm::translate(glm::mat4(1.0f), m_SquareTransform) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5))));

		CatEngine::Renderer::EndScene();

	}

	void OnImGuiDraw() override
	{
		ImGui::Begin("Inspector");

		ImGui::BeginChild("Box Settings", ImVec2(350, 100));
		ImGui::DragInt2("Box Grid", m_GridSize, 0.25, 1, 10);
		ImGui::Checkbox("Show Squares", &renderSquare);
		ImGui::EndChild();
		ImGui::BeginChild("BigBoi Settings");
		ImGui::Checkbox("Show BigBoi", &renderBigCat);
		ImGui::EndChild();

		ImGui::End();
	}

	void OnEvent(CatEngine::Event& event) override 
	{
		m_CameraController.OnEvent(event);

	}
private:

	CatEngine::ShaderLibrary m_ShaderLibrary;

	CatEngine::Ref<CatEngine::VertexArray> m_SquareVertexArray;
	CatEngine::Ref<CatEngine::Texture2D> m_Texture2D;

	CatEngine::OrthographicCameraController m_CameraController;
	float m_ObjectMoveSpeed = 1.0f;

	glm::vec3 m_SquareTransform = { 0,0,0 };

	int m_GridSize[2] = { 1, 1 };

	glm::vec3 squareColor = { 0.8, 0.2, 0.3 };

	bool renderSquare = true;
	bool renderBigCat = false;

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
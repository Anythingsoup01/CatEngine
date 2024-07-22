#include <iostream>

#include "CatEngine.h"
#include "Platform/OpenGL/Renderer/OpenGLShader.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class AppLayer : public CatEngine::Layer {
public:
	AppLayer()
		: Layer("Application"), m_Camera(-1.77, 1.77, -1.0, 1.0, -1.0, 1.0), m_SquareTransform(0.0f)
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

		auto flatColorShader = m_ShaderLibrary.Load("assets/shaders/FlatShader.glsl");
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		
		m_Texture2D = CatEngine::Texture2D::Create("assets/textures/cat_texture.png");
		std::dynamic_pointer_cast<CatEngine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<CatEngine::OpenGLShader>(textureShader)->UploadUniformInt("u_Color", 0);
	}
	
	void OnUpdate(CatEngine::Time time) override 
	{
		//CLI_INFO("DeltaTime: {0}s, ({1}ms)", time.deltaTime(), time.deltaTimeMS());

		// Camera Up
		m_CameraPosition.y += (CatEngine::Input::IsKeyPressed(CE_W)) ? m_CameraSpeed * time.deltaTime() : 0;
		// Camera Down
		m_CameraPosition.y -= (CatEngine::Input::IsKeyPressed(CE_S)) ? m_CameraSpeed * time.deltaTime() : 0;
		// Camera Right
		m_CameraPosition.x += (CatEngine::Input::IsKeyPressed(CE_D)) ? m_CameraSpeed * time.deltaTime() : 0;
		// Camera Left
		m_CameraPosition.x -= (CatEngine::Input::IsKeyPressed(CE_A)) ? m_CameraSpeed * time.deltaTime() : 0;
		// Camera Rotate Right
		m_CameraRotation.z += CatEngine::Input::IsKeyPressed(CE_E) ? m_CameraSpeed * time.deltaTime() : 0;
		// Camera Rotate Left
		m_CameraRotation.z -= CatEngine::Input::IsKeyPressed(CE_Q) ? m_CameraSpeed * time.deltaTime() : 0;
		// Camera Rotate Reset
		m_CameraRotation.z = CatEngine::Input::IsKeyPressed(CE_R) ? 0 : m_CameraRotation.z;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		CatEngine::RenderCommand::Clear({ 0.1, 0.1, 0.1, 1.0 });

		CatEngine::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto flatColorShader = m_ShaderLibrary.Get("FlatShader");
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

		ImGui::BeginChild("Transform", ImVec2(350, 50));
		
		ImGui::DragFloat3("Camera Position", glm::value_ptr(m_CameraPosition), 0.025);
		ImGui::DragFloat3("Camera Rotation", glm::value_ptr(m_CameraRotation), 0.025);
		ImGui::EndChild();

		ImGui::BeginChild("Box Settings", ImVec2(350, 100));
		ImGui::DragInt2("Box Grid", m_GridSize, 0.25, 0, 10);
		ImGui::Checkbox("Show Squares", &renderSquare);
		ImGui::EndChild();
		ImGui::BeginChild("BigBoi Settings");
		ImGui::Checkbox("Show BigBoi", &renderBigCat);
		ImGui::EndChild();

		ImGui::End();
	}

	void OnEvent(CatEngine::Events& event) override 
	{

	}
private:

	CatEngine::ShaderLibrary m_ShaderLibrary;

	CatEngine::Ref<CatEngine::VertexArray> m_SquareVertexArray;
	CatEngine::Ref<CatEngine::Texture2D> m_Texture2D;

	CatEngine::OrthographicCamera m_Camera;

	float m_CameraSpeed = 2.0f;
	float m_ObjectMoveSpeed = 1.0f;

	glm::vec3 m_CameraPosition = { 0, 0, 0 };

	glm::vec3 m_CameraRotation = { 0, 0, 0 };

	glm::vec3 m_SquareTransform;

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
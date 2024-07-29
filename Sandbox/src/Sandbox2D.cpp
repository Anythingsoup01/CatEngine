#include "Sandbox2D.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Application"), m_CameraController(1280.f / 720.f)
{
}

void Sandbox2D::OnAttach()
{
	m_VertexArray = CatEngine::VertexArray::Create();

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
	m_VertexArray->AddVertexBuffer(squareBuffer);

	// Index Buffer
	unsigned int indices[]{ 0, 1, 2, 1, 3, 2 };

	CatEngine::Ref<CatEngine::IndexBuffer> squareIndexBuffer;
	squareIndexBuffer.reset(CatEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(squareIndexBuffer);

	auto flatColorShader = m_ShaderLibrary.Load("FlatColorShader", "assets/shaders/FlatShader.glsl");
	auto name = flatColorShader.get()->GetName();
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(CatEngine::Time time)
{
	// Update
	m_CameraController.OnUpdate(time);

	// Render
	CatEngine::RenderCommand::Clear({ 0.1, 0.1, 0.1, 1.0 });

	CatEngine::Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	auto flatColorShader = m_ShaderLibrary.Get("FlatColorShader");
	flatColorShader->Bind();

	for (int x = 0; x < m_GridSize[0]; x++)
	{
		for (int y = 0; y < m_GridSize[1]; y++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos + m_SquareTransform) * scale;
			CatEngine::Renderer::Submit(flatColorShader, m_VertexArray, squareTransform);
		}
	}

	CatEngine::Renderer::EndScene();
}

void Sandbox2D::OnImGuiDraw()
{
	ImGui::Begin("Inspector");

	ImGui::DragInt2("Box Grid", m_GridSize, 0.25f, 1, 25);

	ImGui::End();
}

void Sandbox2D::OnEvent(CatEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}

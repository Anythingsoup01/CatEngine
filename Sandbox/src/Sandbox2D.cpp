#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#define PROFILE(name) PROFILE_SCOPE(name, [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult);})

Sandbox2D::Sandbox2D()
	: Layer("Application"), m_CameraController(1280.f / 720.f)
{
}

void Sandbox2D::OnAttach()
{
	m_CatTexture = CatEngine::Texture2D::Create("assets/textures/cat_texture.png");
	m_CheckeredTexture = CatEngine::Texture2D::Create("assets/textures/checkered.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(CatEngine::Time time)
{
	CE_PROFILE_FUNCTION();
	// Update
	{
		CE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(time);
	}
	// Render
	{
		CE_PROFILE_SCOPE("Prerender Commands");
		CatEngine::RenderCommand::Clear({ 0.1, 0.1, 0.1, 1.0 });
	}
<<<<<<< HEAD
	{
		CE_PROFILE_SCOPE("Render");
		CatEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		CatEngine::Renderer2D::DrawQuad({ 1, 1, -.1f }, 0, { 10, 10 }, m_CatTexture, m_BoxOne, m_TexTile);
		CatEngine::Renderer2D::DrawQuad({ 0, 0, -.1f }, 0, { 1, 1 }, m_CatTexture, glm::vec4(1.0f), 10);

=======
	CatEngine::Renderer2D::ResetStats();
	{
		CE_PROFILE_SCOPE("Render");

		CatEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float x = -10.f; x < 10.f; x += .1f)
		{
			for (float y = -10.f; y < 10.f; y += .1f)
			{
				glm::vec2 color = { (x + 10.f) / 20.f, (y + 10.f) / 20.f };
				CatEngine::Renderer2D::DrawQuad({ x , y }, 0, { .1f, .1f }, { color.r, 1.f, color.g, 1.f });
			}
		}

>>>>>>> e0fb026 (Improved Batch Renderer)
		CatEngine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiDraw()
{
	CE_PROFILE_FUNCTION();

	ImGui::Begin("Inspector");

<<<<<<< HEAD

	{
		CE_PROFILE_SCOPE("ImGui Children");
		ImGui::ColorEdit4("Box One", glm::value_ptr(m_BoxOne));
		ImGui::DragFloat("Tile Texture", &m_TexTile, .25, 1, 100);
	}

=======
	{
		CE_PROFILE_SCOPE("ImGui Children");

		CatEngine::Renderer2D::Statistics stats = CatEngine::Renderer2D::GetStats();
		ImGui::Text("Render2D Stats:");
		ImGui::Text("Draw Calls: %i", stats.DrawCalls);
		ImGui::Text("Quads: %i", stats.QuadCount);
		ImGui::Text("Vertex Count: %i", stats.GetTotalVertexCount());
		ImGui::Text("Index Count: %i", stats.GetTotalIndexCount());
	}
>>>>>>> e0fb026 (Improved Batch Renderer)

	ImGui::End();

}

void Sandbox2D::OnEvent(CatEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}


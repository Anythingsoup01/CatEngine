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
	m_CameraController.SetZoomLevel(5.f);

    m_CheckeredTexture = CatEngine::Texture2D::Create("assets/textures/cat_texture.png");

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
	CatEngine::Renderer2D::ResetStats();
	{
		CE_PROFILE_SCOPE("Render");

		CatEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (float y = -5.f; y < 5.f; y += .5f)
		{
			for (float x = -5.f; x < 5.f; x += .5f) 
			{
				glm::vec2 color = { {(x + 5.f) / 10.f}, {(y + 5.f) / 10.f} };
				CatEngine::Renderer2D::DrawQuad({ x, y }, 0, { .45f, .45f }, { color.x, 1.f, color.y, 1.f });
			}
		}

		CatEngine::Renderer2D::EndScene();
	}
}
void Sandbox2D::OnImGuiDraw()
{
    CE_PROFILE_FUNCTION();

   
    ImGui::Begin("Scene");

	auto stats = CatEngine::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices:", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::End();


}

void Sandbox2D::OnEvent(CatEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}


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
	{
		CE_PROFILE_SCOPE("Render");
		CatEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		CatEngine::Renderer2D::DrawQuad({ 0, 0, -.1f }, 0, { 1, 1 }, m_CheckeredTexture, m_BoxOne, m_TexTile.x);
		CatEngine::Renderer2D::DrawQuad({ 0, 0, 0 }, 0, { .5, .75 }, m_CatTexture);

		CatEngine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiDraw()
{
	CE_PROFILE_FUNCTION();

	ImGui::Begin("Inspector");


	{
		CE_PROFILE_SCOPE("ImGui Children");
		ImGui::ColorEdit4("Box One", glm::value_ptr(m_BoxOne));
		ImGui::DragFloat("Tile Texture", glm::value_ptr(m_TexTile), .25, 1, 100);
	}


	ImGui::End();

}

void Sandbox2D::OnEvent(CatEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}
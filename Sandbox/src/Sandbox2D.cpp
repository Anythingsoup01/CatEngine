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
	m_Texture = CatEngine::Texture2D::Create("assets/textures/cat_texture.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(CatEngine::Time time)
{
	PROFILE("Sandbox::OnUpdate");
	// Update
	{
		PROFILE("Sandbox2D::CameraController::OnUpdate");
		m_CameraController.OnUpdate(time);
	}
	// Render
	{
		PROFILE("Sandbox2D::PrerenderCommands");
		CatEngine::RenderCommand::Clear({ 0.1, 0.1, 0.1, 1.0 });
	}
	{
		PROFILE("Sandbox2D::Render");
		CatEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		CatEngine::Renderer2D::DrawQuad({ 0, 0, 0 }, 0, { .5, .75 }, m_Texture, m_BoxOne, m_TexTile.x);
		CatEngine::Renderer2D::DrawQuad({ 0, 0, -.2 }, 0, { 1, 1 }, m_BoxTwo);

		CatEngine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiDraw()
{
	ImGui::Begin("Inspector");

	ImGui::ColorEdit4("Box One", glm::value_ptr(m_BoxOne));
	ImGui::DragFloat("Tile Texture", glm::value_ptr(m_TexTile));
	ImGui::ColorEdit4("Box Two", glm::value_ptr(m_BoxTwo));

	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, " %.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();
	
	ImGui::End();
}

void Sandbox2D::OnEvent(CatEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}
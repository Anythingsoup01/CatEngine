#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Application"), m_CameraController(1280.f / 720.f)
{
}

void Sandbox2D::OnAttach()
{

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

	CatEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	CatEngine::Renderer2D::DrawQuad({ 1, 1 }, 0, { .5, .75 }, m_BoxOne);
	CatEngine::Renderer2D::DrawQuad({ 0, 0 }, 0, { 1, 1 }, m_BoxTwo);

	CatEngine::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiDraw()
{
	ImGui::Begin("Inspector");

	ImGui::ColorEdit4("Box One", glm::value_ptr(m_BoxOne));
	ImGui::ColorEdit4("Box Two", glm::value_ptr(m_BoxTwo));
	
	ImGui::End();
}

void Sandbox2D::OnEvent(CatEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}
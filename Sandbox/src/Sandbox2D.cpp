#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#define PROFILE(name) PROFILE_SCOPE(name, [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult);})

static const uint32_t s_MapWidth = 37;
static const char* s_MapTiles = 
"0000000000000000000000000000000000000"
"00000000000000000000000000000000SSSS0"
"0000000000000000000000000000000SSSSS0"
"000000000000000000000000000000SSSSSSS"
"00000000000000000000000000000SSSSSSSS"
"0000000000000000000000000000SSSSSSSSS"
"0000000000000000000000000000SSSFFSSSS"
"000000000000000000000000000SSSSFFFSSS"
"000000000000000000000000000SSSFFFFFSS"
"00000000000000000000000000SSSFFFFFFSS"
"000000SSSSSSSS000000000000SSFFFFFFFSS"
"0000SSSSSSSSSSS00000000000SSFFFFFFFSS"
"00SSSSSSSSSSSSSS0000000000SFFFFFFFFSS"
"SSSSSSSSSSSFFFFSS000000000SFFFFFFFFSS"
"SSSSSSSSFFFFFFFFSS00000000SFFFFFFFFSS"
"SSSSSSFFFFFFFFFFFS00SSSS00SSFFFFFFFSS"
"SSSSSSSFFFFFFFFFFSSSSSSSSSSSFFFFFFFSS"
"SSSSSSSFFFFFFFFFFSSFFFFFFFSSFFFFFFFSS"
"0SSSSSFFFFFFFFFFFFFFFFFFFFFFFFFFFFSS0"
"00SSSSSFFFFFFFFFFFFFFFFFFFFFFFFFFSS00"
"000SSSSSFFFFFFFFFFFFFFFFFFFFFFFFSS000"
"0000SSSSSSFFFFFFFFFFFFFFFFFFFFFSS0000"
"00000SSSSSSFFFFFFSSFFFFFFFFFSSFSS0000"
"000000SSSSSSSSFFSEESFFFFFFFSEESFS0000"
"00000000SSSSSFFFSEESFFFFFFFSEESFS0000"
"000000000000SFFFSSSSFFFFFFFSSSSFSS000"
"000000000000SFFFSSSSFFFFSFFSSSSFSS000"
"00000000000SSFFFSSSFFFFFFFFFSSSFFS000"
"00000000000SSFFFFFFFFSSSSSSFFFFFFS000"
"00000000000SSFSSSFFFFSSSSSSFFFSSSS000"
"00000000000SSSSFSSFFFSSSSSSFFSFFSS000"
"000000000000SSFFFSFFFSSSSSSFFSFFSS000"
"000000000000SSFFFSFFFSSSSSSFFSFFS0000"
"000000000000SSFFFSFFFSSSSSSFFSFFS0000"
"0000000000000SSFSSFFFFSSSSFFFSFS00000"
"000000000000SSSSFFFFFFFFFFFFFFSS00000"
"0000000000SSSSSSFFFFFFFFFFFFFSS000000"
"0000000000SSFFFFSFFFFFFFFFFFSSSSS0000"
"000000000SSSFFFFFSSFFFFFFFSSFFFSSS000"
"0000000000SFFFFFFSSSSSSSSSSFFFFFSS000"
"0000000000SSFFFFFFSSSSSSSSSFFFFFSS000"
"000000000SSSSFFFFFSSSSSSSSSFFFFSS0000"
"00000000SSSSSSSFFFSSSFSSSSFFFFSS00000"
"000000SSSSSSSSSSSFFSSFFFSFFFSSS000000"
"00000SSSSSSSSSSFFFFSFFFFSFSSS00000000"
"0000SSSSSSSSSSSFFFFFFFFFFFSS000000000"
"0000SSSSSSSS00SSFFFFFFFFFFS0000000000"
"00000SSSSSS00SSSFFFFFFFFFFS0000000000"
"000000SSSS00SSFFSFFFFFFFFFS0000000000"
"000000SSS00SSFFFFSFFFFFFFSS0000000000"
"00000000000SSFFFFSSSSFFFSFS0000000000"
"00000000000SFFFFFS00SSSSFFSS000000000"
"00000000000SFSFSFS0000SFFFFS000000000"
"000000000000SSSSS00000SFFFFS000000000"
"0000000000000000000000SFFFFS000000000"
"0000000000000000000000SFFFFS000000000"
"00000000000000000000000SFSFS000000000"
"000000000000000000000000SSS0000000000"

;

Sandbox2D::Sandbox2D()
	: Layer("Application"), m_CameraController(1280.f / 720.f)
{
}

void Sandbox2D::OnAttach()
{
	m_CameraController.SetZoomLevel(5.f);

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
	CatEngine::Renderer2D::ResetStats();
	{
		CE_PROFILE_SCOPE("Render");

		CatEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (float y = -10.f; y < 10.f; y += .5f)
		{
			for (float x = -10.f; x < 10.f; x += .5f) 
			{
				glm::vec2 color = { {(x + 10.f) / 20.f}, {(y + 10.f) / 20.f} };
				CatEngine::Renderer2D::DrawQuad({ x, y }, 0, { .45f, .45f }, { color.x, 1.f, color.y, 1.f });
			}
		}

		CatEngine::Renderer2D::EndScene();
	}
}
void Sandbox2D::OnImGuiDraw()
{
    CE_PROFILE_FUNCTION();

    static bool dockspaceOpen = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    ImGui::Begin("Scene");

    ImGui::Image((void*)m_CheckeredTexture->GetRendererID(), ImVec2(64.f, 64.f));
    ImGui::End();

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                CatEngine::Application::Get().CloseEditor();

            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void Sandbox2D::OnEvent(CatEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}


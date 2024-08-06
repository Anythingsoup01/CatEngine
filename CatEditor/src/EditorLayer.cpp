#include "EditorLayer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#define PROFILE(name) PROFILE_SCOPE(name, [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult);})

namespace CatEngine 
{
    EditorLayer::EditorLayer()
        : Layer("Application"), m_CameraController(1280.f / 720.f)
    {
    }

    void EditorLayer::OnAttach()
    {
        m_CameraController.SetZoomLevel(5.f);

        FrameBufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);

    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(Time time)
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
            m_FrameBuffer->Bind();
            RenderCommand::Clear({ 0.1, 0.1, 0.1, 1.0 });
        }
        Renderer2D::ResetStats();
        {
            CE_PROFILE_SCOPE("Render");

            Renderer2D::BeginScene(m_CameraController.GetCamera());

            for (float y = -5.f; y < 5.f; y += .5f)
            {
                for (float x = -5.f; x < 5.f; x += .5f)
                {
                    glm::vec2 color = { {(x + 5.f) / 10.f}, {(y + 5.f) / 10.f} };
                    Renderer2D::DrawQuad({ x, y }, 0, { .45f, .45f }, { color.x, 1.f, color.y, 1.f });
                }
            }

            Renderer2D::EndScene();
            m_FrameBuffer->Unbind();
        }
    }
    void EditorLayer::OnImGuiDraw()
    {
        CE_PROFILE_FUNCTION();

        static bool dockspaceOpen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar(3);
        {
            // Submit the DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Exit"))
                    {
                        Application::Get().CloseEditor();

                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

            ImGui::Begin("Scene");
            {
                ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
                if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
                {
                    m_FrameBuffer->SetSize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
                    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

                    m_CameraController.OnResizeBounds(viewportPanelSize.x, viewportPanelSize.y);
                }

                uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
                ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
            }
            ImGui::PopStyleVar();
            ImGui::End();

        }
        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_CameraController.OnEvent(e);
    }

}
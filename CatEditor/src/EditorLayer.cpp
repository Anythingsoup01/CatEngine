#include "EditorLayer.h"

#include "CatEngine/Utils/PlatformUtils.h"

#include "glm/gtc/type_ptr.hpp"

#include <imgui.h>
#include <ImGuizmo.h>

#include "CatEngine/Math/Math.h"

#define PROFILE(name) PROFILE_SCOPE(name, [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult);})

namespace CatEngine 
{
    EditorLayer::EditorLayer()
        : Layer("Application"), m_CameraController(1280.f / 720.f)
    {
    }

    void EditorLayer::OnAttach()
    {
        FrameBufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);

        // Entity
        m_ActiveScene = CreateRef<Scene>();
    
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(Time time)
    {
        CE_PROFILE_FUNCTION();

        if (m_Minimized)
        {
            m_CameraController.OnResizeBounds(m_ViewportSize.x, m_ViewportSize.y);
            m_Minimized = false;
        }

        if (m_ViewportFocused && m_ViewportHovered)
        {
        }

        //Resize

        if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_FrameBuffer->SetSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResizeBounds(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
        // Render
        Renderer2D::ResetStats();

        m_FrameBuffer->Bind();
        RenderCommand::Clear({ 0.1, 0.1, 0.1, 1.0 });

        // Update Scene
        m_ActiveScene->OnUpdate(time);

        m_FrameBuffer->Unbind();
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
            ImGuiStyle& style = ImGui::GetStyle();
            style.WindowMinSize.x = 360.f;
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("New", "Ctrl+N")) NewScene();

                    if (ImGui::MenuItem("Open...", "Ctrl+O")) OpenScene();

                    if (ImGui::MenuItem("Save", "Ctrl+S")) SaveScene();
                    
                    if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) SaveSceneAs();
                   

                    if (ImGui::MenuItem("Exit" )) Application::Get().CloseEditor();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }



            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
            ImGui::Begin("Scene");
            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);
            {
                ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

                uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
                ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
            }


            // Gizmos
            Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
            if (selectedEntity && m_GizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                float windowWidth = (float)ImGui::GetWindowWidth();
                float windowHeight = (float)ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                // Camera

               auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
                if (cameraEntity)
                {
                    const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
                    const glm::mat4& cameraProjection = camera.GetProjection();
                    glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

                    // Entity transform
                    auto& tc = selectedEntity.GetComponent<TransformComponent>();
                    glm::mat4 transform = tc.GetTransform();

                    // Snapping
                    bool snap = Input::IsKeyPressed(Key::LeftControl);
                    float snapValue = 0.5f; // Snap to 0.5m for translation/scale
                    // Snap to 45 degrees for rotation
                    if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                        snapValue = 45.0f;

                    float snapValues[3] = { snapValue, snapValue, snapValue };

                    ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

                    if (ImGuizmo::IsUsing())
                    {
                        glm::vec3 position, rotation, scale;
                        Math::DecomposeTransform(transform, position, rotation, scale);

                        glm::vec3 deltaRotation = rotation - tc.Rotation;
                        tc.Position = position;
                        tc.Rotation += deltaRotation;
                        tc.Scale = scale;

                    }
                }
            }
            ImGui::End();

            m_SceneHierarchyPanel.OnImGuiRender();

            ImGui::Begin("Console");
            {

            }
            ImGui::End();


            ImGui::PopStyleVar();

        }
        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_CameraController.OnEvent(e);
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(EditorLayer::OnWindowResize));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        return false;
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {   
        // Shortcuts
        if (e.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
        bool shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);
        switch (e.GetKeyCode())
        {
            case (int)KeyCode::S :
            {
                if (control && shift)
                    SaveSceneAs();
                else if (control)
                    SaveScene();
                break;
            }
            case (int)KeyCode::O:
            {
                if (control)
                    OpenScene();
                break;
            }
            case (int)KeyCode::N:
            {
                if (control) 
                    NewScene();
                break;
            }
            case (int)KeyCode::D1:
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            }
            case (int)KeyCode::D2:
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            }
            case (int)KeyCode::D3:
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
            }
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        m_SceneFilePath = FileDialogs::SaveFile("CatEngine Scene (*.catengine)\0*.catengine\0");
        if (!m_SceneFilePath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);

            serializer.Serialize(m_SceneFilePath);
        }
    }

    void EditorLayer::SaveScene()
    {
        if (!m_SceneFilePath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);

            serializer.Serialize(m_SceneFilePath);
        }
        else SaveSceneAs();
    }

    void EditorLayer::OpenScene()
    {
        m_SceneFilePath = FileDialogs::OpenFile("CatEngine Scene (*.catengine)\0*.catengine\0");
        if (!m_SceneFilePath.empty())
        {
            m_ActiveScene = CreateRef<Scene>();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);

            serializer.Deserialize(m_SceneFilePath);
        }
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }
    

}
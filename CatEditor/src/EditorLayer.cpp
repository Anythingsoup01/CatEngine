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

        m_IconStartRuntime = Texture2D::Create("Resources/Icons/Editor/Start-Runtime.png");
        m_IconStopRuntime = Texture2D::Create("Resources/Icons/Editor/Stop-Runtime.png");

        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        // Entity
        m_ActiveScene = CreateRef<Scene>();

        auto commandLineArgs = Application::Get().GetCommandLineArgs();
        if (commandLineArgs.Count > 1)
        {
            auto sceneFilePath = commandLineArgs[1];
            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(sceneFilePath);
        }
        
        m_EditorCamera = EditorCamera(30.f, 1.778f, 0.1f, 1000.f);

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(Time time)
    {
        CE_PROFILE_FUNCTION();

        //Resize

        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
        
        // Render
        Renderer2D::ResetStats();

        m_Framebuffer->Bind();
        RenderCommand::Clear({ 0.1, 0.1, 0.1, 1.0 });

        m_Framebuffer->ClearAttachment(1, -1);

        // Update Scene
        switch (m_SceneState)
        {
            case SceneState::Edit:
            {
                // Update Camera
                if (!ImGuizmo::IsUsing())
                    m_EditorCamera.OnUpdate(time);

                m_ActiveScene->OnUpdateEditor(time, m_EditorCamera);
                break;
            }
            case SceneState::Play:
            {
                m_ActiveScene->OnUpdateRuntime(time);
                break;
            }
        }

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;


        int mouseX = (int)mx;
        int mouseY = (int)my;

        if ((mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y))
        {
            int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
            m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
        }


        m_Framebuffer->Unbind();
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
        ImGui::Begin("MyDockSpace", &dockspaceOpen, window_flags);
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


                    if (ImGui::MenuItem("Exit")) Application::Get().CloseEditor();
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Scene"))
                {
                    const char* runtimeText = m_SceneState == SceneState::Edit ? "Start Runtime" : "Stop Runtime";
                    if (ImGui::MenuItem(runtimeText, "Ctrl+F5"))  m_SceneState == SceneState::Edit ? OnScenePlay() : OnSceneStop();

                    if (ImGui::MenuItem("Pause Runtime", "Ctrl+Shift+F5")) OnScenePause();

                    if (ImGui::MenuItem("Simulate Runtime", "Ctrl+F7")) OnSceneSimulate();

                    if (ImGui::MenuItem("Exit")) Application::Get().CloseEditor();
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
            ImGui::Begin("Scene");
            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();
            m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };



            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);
            
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
            ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_MANAGER_ITEM"))
                {
                    const wchar_t* path = (const wchar_t*)payload->Data;
                    std::filesystem::path filePath = std::filesystem::path("assets") / path;

                    OpenScene(filePath);
                    m_SceneFilePath = filePath.string();
                }

                ImGui::EndDragDropTarget();
            }


            // Gizmos
            Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
            if (selectedEntity && m_GizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);


                // Camera

                if (m_SceneState == SceneState::Edit)
                {
                    const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
                    glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();
                    ImGuizmoDraw(selectedEntity, cameraProjection, cameraView);
                }


            }
            ImGui::End();

            m_SceneHierarchyPanel.OnImGuiRender();
            m_ContentBrowserPanel.OnImGuiRender();

            ImGui::Begin("Console");
            {

            }
            ImGui::End();
            ImGui::PopStyleVar();

            UI_Toolbar();

        }
        ImGui::End();
    }


    void EditorLayer::ImGuizmoDraw(Entity selectedEntity, const glm::mat4& cameraProjection, glm::mat4 cameraView)
    {
        // Entity transform
        auto& tc = selectedEntity.GetComponent<TransformComponent>();
        glm::mat4 transform = tc.GetTransform();

        // Snapping
        bool snap = Input::IsKeyPressed(KeyCode::LeftControl);
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

    void EditorLayer::UI_Toolbar()
    {

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 2 });
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0, 0 });

        ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        {
            float size = ImGui::GetWindowHeight() - 4.f;
            Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconStartRuntime : m_IconStopRuntime;
            ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f - (size * 0.5f)));
            if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), {0, 0}, {1, 1}, 0))
            {
                if (m_SceneState == SceneState::Edit)
                    OnScenePlay();
                else if (m_SceneState == SceneState::Play)
                    OnSceneStop();
            }
            ImGui::PopStyleVar(2);
        }
        ImGui::End();

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
            case (int)KeyCode::F5:
            {
                if (control && shift)
                    OnScenePause();
                else if (control)
                    m_SceneState == SceneState::Edit ? OnScenePlay() : OnSceneStop();
                break;
            }
            case (int)KeyCode::F7:
            {
                if (control)
                    OnSceneSimulate();
                break;
            }
            case (int)KeyCode::Q:
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            }
            case (int)KeyCode::W:
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            }
            case (int)KeyCode::E:
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
            }
            default:
                break;
        }
        return false;
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == MouseCode::ButtonLeft)
        {
            if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(KeyCode::LeftAlt) && m_SceneState == SceneState::Edit)
                m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
        }
        return false;
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
            OpenScene(m_SceneFilePath);
    }
    void EditorLayer::OpenScene(const std::filesystem::path& filePath)
    {
        if (filePath.extension().string() != ".catengine")
        {
            CE_API_WARN("Could not load {0} - not a scene file", filePath.filename().string());
            return;
        }

        Ref<Scene> newScene = CreateRef<Scene>();
        SceneSerializer serializer(newScene);
        if (serializer.Deserialize(filePath.string()))
        {
            m_ActiveScene = newScene;
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        }
    }

    void EditorLayer::NewScene()
    {
        m_SceneFilePath = "";
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_EditorCamera.OnEvent(e);
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(EditorLayer::OnWindowResize));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

    void EditorLayer::OnScenePlay()
    {
        m_ActiveScene->OnRuntimeStart();
        m_SceneState = SceneState::Play;
    }

    void EditorLayer::OnSceneStop()
    {
        m_ActiveScene->OnRuntimeStop();
        m_SceneState = SceneState::Edit;
    }
}
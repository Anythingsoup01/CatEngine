#include "EditorLayer.h"

#include <imgui.h>

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

        m_Texture = Texture2D::Create("assets/textures/checkered.png");

        // Entity

        m_ActiveScene = CreateRef<Scene>();

        m_SquareEntity = m_ActiveScene->CreateEntity("Square Entity");
        m_SquareEntity.AddComponent<SpriteRendererComponent>(m_SquareColor);

        m_SecondSquareEntity = m_ActiveScene->CreateEntity("Square Entity");
        m_SecondSquareEntity.AddComponent<SpriteRendererComponent>(m_SquareColor);

        m_MainCameraEntity = m_ActiveScene->CreateEntity("Main Camera");
        m_MainCameraEntity.AddComponent<CameraComponent>().Primary = false;

        m_SceneCameraEntity = m_ActiveScene->CreateEntity("Scene Camera");
        m_SceneCameraEntity.AddComponent<CameraComponent>();


        class SceneCameraController : public SoloAction
        {
        public:

            void Update(Time time)
            {
                auto& transform = GetComponent<TransformComponent>().Transform;

                float cameraSpeed = 5.f;

                transform[3][1] += Input::IsKeyPressed(KeyCode::W) ? cameraSpeed * time.deltaTime() : 0;
                transform[3][1] += Input::IsKeyPressed(KeyCode::S) ? -cameraSpeed * time.deltaTime() : 0;
                transform[3][0] += Input::IsKeyPressed(KeyCode::D) ? cameraSpeed * time.deltaTime() : 0;
                transform[3][0] += Input::IsKeyPressed(KeyCode::A) ? -cameraSpeed * time.deltaTime() : 0;

            }
        };
        m_SceneCameraEntity.AddComponent<NativeScriptComponent>().Bind<SceneCameraController>();

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
            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);
            {
                ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

                uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
                ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
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

    

}
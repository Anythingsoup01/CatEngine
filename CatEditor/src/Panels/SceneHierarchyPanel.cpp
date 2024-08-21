#include "SceneHierarchyPanel.h"
#include "CatEngine/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace CatEngine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");
		for (auto entityID : m_Context->m_Registry.view<entt::entity>())
		{
			Entity entity{ entityID , m_Context.get() };
			DrawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}


		ImGui::End();
		ImGui::Begin("Inspector");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetComponent<NameComponent>().Name;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		
		if (opened)
		{
			ImGui::TreePop();
		}

	}
	void SceneHierarchyPanel::DrawComponents(Entity selection)
	{
		{

			auto& tag = selection.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}

			auto& name = selection.GetComponent<NameComponent>().Name;

			char nameBuffer[256];
			memset(nameBuffer, 0, sizeof(nameBuffer));
			strcpy_s(nameBuffer, name.c_str());
			if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
			{
				name = std::string(nameBuffer);
			}

		}

		if (selection.HasComponent<TransformComponent>())
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "Transform"))
			{
				auto& transform = selection.GetComponent<TransformComponent>().Transform;

				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
				ImGui::TreePop();
			}

		}
		if (selection.HasComponent<SpriteRendererComponent>())
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), flags, "Sprite"))
			{
				auto& spriteColor = selection.GetComponent<SpriteRendererComponent>().Color;

				ImGui::ColorEdit4("Color", glm::value_ptr(spriteColor), 0.1f);
				ImGui::TreePop();
			}
		}
		if (selection.HasComponent<NativeScriptComponent>())
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
			if (ImGui::TreeNodeEx((void*)typeid(NativeScriptComponent).hash_code(), flags, "Script"))
			{
				auto& spriteColor = selection.GetComponent<NativeScriptComponent>().Instance;

				ImGui::TreePop();
			}
		}
		if (selection.HasComponent<CameraComponent>())
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), flags, "Camera"))
			{
				auto& cameraComponent = selection.GetComponent<CameraComponent>();
				auto& camera = selection.GetComponent<CameraComponent>().Camera;

				const char* projectionTypeString[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeString[i];
						if (ImGui::Selectable(projectionTypeString[i], &isSelected))
						{
							currentProjectionTypeString = projectionTypeString[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
						
					}
					ImGui::EndCombo();
				}
				auto& primary = cameraComponent.Primary;
				ImGui::Checkbox("Primary Camera", &primary);

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspectice)
				{
					auto vertFov = glm::degrees(camera.GetPerspectiveVerticalFov());
					if (ImGui::DragFloat("Vertical FOV", &vertFov))
						camera.SetPerspectiveVerticalFov(glm::radians(vertFov));

					auto nearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip, .01f, 0.f))
						camera.SetPerspectiveNearClip(nearClip);

					auto farClip = camera.GetPerspectivecFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip, .01f, 0.f))
						camera.SetPerspectiveFarClip(farClip);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					auto size = camera.GetOrthographicSize();
					if (ImGui::DragFloat("FOV", &size))
						camera.SetOrthographicSize(size);
					
					auto nearClip = camera.GetOrthographicNearClip();
					if(ImGui::DragFloat("Near Clip", &nearClip, .01f, 0.f))
						camera.SetOrthographicNearClip(nearClip);

					auto farClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip, .01f, 0.f))
						camera.SetOrthographicFarClip(farClip);
				}

				ImGui::TreePop();
			}
		}
	}
}
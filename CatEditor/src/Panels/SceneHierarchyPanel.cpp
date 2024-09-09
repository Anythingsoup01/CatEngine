#include "SceneHierarchyPanel.h"
#include "CatEngine/Scene/Components.h"
#include "ImGui/ImGuiDraw.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>

namespace CatEngine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_SelectionContext = {};
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

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty GameObject"))
			{
				auto& entity = m_Context->CreateEntity("GameObject");
				m_SelectionContext = entity;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Create Camera"))
			{
				auto& entity = m_Context->CreateEntity("Camera");
				entity.AddComponent<CameraComponent>().Primary = false;
				m_SelectionContext = entity;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::End();
		ImGui::Begin("Inspector");
		if (m_SelectionContext)
		{
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			DrawComponents(m_SelectionContext);

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (((float)contentRegionAvailable.x - ((float)contentRegionAvailable.x / 2)) * .5));

			bool Components2DAvaliable = ((!m_SelectionContext.HasComponent<SpriteRendererComponent>() && !m_SelectionContext.HasComponent<CircleRendererComponent>()) || !m_SelectionContext.HasComponent<Rigidbody2DComponent>() || (!m_SelectionContext.HasComponent<BoxCollider2DComponent>() && !m_SelectionContext.HasComponent<CircleCollider2DComponent>()));

			if (ImGui::Button("Add Component", ImVec2{ contentRegionAvailable.x / 2.f, lineHeight }))
				ImGui::OpenPopup("AddComponent");
			
			if (ImGui::BeginPopup("AddComponent"))
			{
				DisplayAddComponentEntry<CameraComponent>("Camera Component");
				if (Components2DAvaliable)
				{
					bool opened = ImGui::TreeNodeEx("2D Components", treeNodeFlags);

					if (opened)
					{
						DisplayAddComponentEntries<SpriteRendererComponent, CircleRendererComponent>("Sprite Renderer Component", "Circle Renderer Component");
						DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
						DisplayAddComponentEntries<BoxCollider2DComponent, CircleCollider2DComponent>("Box Collider 2D", "Circle Collider 2D");
						ImGui::TreePop();
					}
				}
				ImGui::EndPopup();
			}

		}
		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetComponent<NameComponent>().Name;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
				entityDeleted = true;

			ImGui::EndPopup();
		}
		
		if (opened)
		{
			ImGui::TreePop();
		}
		if (entityDeleted)
		{
			m_Context->DeleteEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, bool canRemove = true)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();


			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 10, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight);
			if (ImGui::Button(":", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("Component Settings");
			}
			bool removeComponent = false;
			if (ImGui::BeginPopup("Component Settings"))
			{
				if (canRemove)
				{
					if (ImGui::MenuItem("Remove Component"))
					{
						removeComponent = true;
						ImGui::CloseCurrentPopup();
					}
				}
				if (ImGui::MenuItem("Reset Component"))
				{
					component.ResetComponent();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity selection)
	{
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 10,1 });
			ImGui::Columns(2);

			auto& tag = selection.GetComponent<TagComponent>().Tag;

			char tagBuffer[256];
			memset(tagBuffer, 0, sizeof(tagBuffer));
			strcpy_s(tagBuffer, tag.c_str());
			ImGui::Text("Tag");
			ImGui::SameLine();
			if (ImGui::InputText("##T", tagBuffer, sizeof(tagBuffer)))
			{
				tag = std::string(tagBuffer);
			}

			ImGui::NextColumn();

			auto& layer = selection.GetComponent<LayerComponent>().Layer;

			char layerBuffer[256];
			memset(layerBuffer, 0, sizeof(layerBuffer));
			strcpy_s(layerBuffer, layer.c_str());
			ImGui::Text("Layer");
			ImGui::SameLine();
			if (ImGui::InputText("##L", layerBuffer, sizeof(layerBuffer)))
			{
				layer = std::string(layerBuffer);
			}

			ImGui::Columns(1);

			auto& name = selection.GetComponent<NameComponent>().Name;

			char nameBuffer[256];
			memset(nameBuffer, 0, sizeof(nameBuffer));
			strcpy_s(nameBuffer, name.c_str());
			ImGui::Text("Name");
			ImGui::SameLine();
			if (ImGui::InputText("##N", nameBuffer, sizeof(nameBuffer)))
			{
				name = std::string(nameBuffer);
			}
			ImGui::PopStyleVar();
		}

		DrawComponent<TransformComponent>("Transform", selection, [](auto& component) {
			ImGuiDraw::DrawVec3Control("Position", component.Position, 0.1f, 0.0f, 0.0f);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			ImGuiDraw::DrawVec3Control("Rotation", rotation, 0.1f, 0.0f, 0.0f);
			component.Rotation = glm::radians(rotation);
			ImGuiDraw::DrawVec3Control("Scale", component.Scale, 0.1f, 0.0f, 0.0f);
		}, false);
		 
		DrawComponent<SpriteRendererComponent>("Sprite Renderer", selection, [](auto& component) 
		{
			ImGuiDraw::DrawColorEdit4Control("Color", component.Color);
			
			ImGui::Button("Texture", ImVec2{100.f, 100.f});
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_MANAGER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path("assets") / path;
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					if (texture->IsLoaded())
						component.Texture = texture;
					else
						CE_API_WARN("Could not load texture {0}", texturePath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}

			ImGuiDraw::DrawVec1Control("Tiling Factor", component.TilingFactor, 0.1f, 0.f, 100.f);
		});

		DrawComponent<CircleRendererComponent>("Circle Renderer", selection, [](auto& component)
		{
			ImGuiDraw::DrawColorEdit4Control("Color", component.Color);

			ImGuiDraw::DrawVec1Control("Thickness", component.Thickness, 0.025f, 0.f, 1.f);
			ImGuiDraw::DrawVec1Control("Fade", component.Fade, 0.00025f, 0.f, 1.f);
		});

		DrawComponent<CameraComponent>("Camera", selection, [](auto& component) 
		{
			auto& camera = component.Camera;

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
			auto& primary = component.Primary;
			ImGui::Checkbox("Primary Camera", &primary);

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspectice)
			{
				auto vertFov = glm::degrees(camera.GetPerspectiveVerticalFov());
				if (ImGui::DragFloat("Vertical FOV", &vertFov))
					camera.SetPerspectiveVerticalFov(glm::radians(vertFov));

				auto nearClip = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near Clip", &nearClip, .01f, 0.f))
					camera.SetPerspectiveNearClip(nearClip);

				auto farClip = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far Clip", &farClip, .01f, 0.f))
					camera.SetPerspectiveFarClip(farClip);
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				auto size = camera.GetOrthographicSize();
				if (ImGui::DragFloat("FOV", &size))
					camera.SetOrthographicSize(size);

				auto nearClip = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near Clip", &nearClip, .01f, 0.f))
					camera.SetOrthographicNearClip(nearClip);

				auto farClip = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far Clip", &farClip, .01f, 0.f))
					camera.SetOrthographicFarClip(farClip);
			}
		});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", selection, [](auto& component)
			{
				const char* bodyTypeString[] = { "Static", "Kinematic", "Dynamic" };
				const char* currentBodyTypeString = bodyTypeString[(int)component.Type];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeString[i];
						if (ImGui::Selectable(bodyTypeString[i], &isSelected))
						{
							currentBodyTypeString = bodyTypeString[i];
							component.Type = (Rigidbody2DComponent::BodyType)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();

					}
					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
			});

		DrawComponent<BoxCollider2DComponent>("BoxCollider 2D", selection, [](auto& component)
			{
				glm::vec2& offset = component.Offset;
				glm::vec2& size = component.Size;

				float& density = component.Density;
				float& friction = component.Friction;
				float& restitution = component.Restitution;
				float& restitutionThreshold = component.RestitutionThreshold;

				ImGuiDraw::DrawVec2Control("Offset", component.Offset);
				ImGuiDraw::DrawVec2Control("Size", component.Size, 0.025f, 0.0001, 100000.f);

				ImGuiDraw::DrawVec1Control("Density", component.Density, 0.025f, 0.0001, 100000.f);
				ImGuiDraw::DrawVec1Control("Friction", component.Friction, 0.025f, 0.0001, 100000.f);
				ImGuiDraw::DrawVec1Control("Restitution", component.Restitution, 0.025f, 0.0001, 100000.f);
				ImGuiDraw::DrawVec1Control("Restitution Threshold", component.RestitutionThreshold, 0.025f, 0.0001, 100000.f);
			});

		DrawComponent<CircleCollider2DComponent>("CircleCollider 2D", selection, [](auto& component)
			{
				glm::vec2& offset = component.Offset;
				float& size = component.Radius;

				float& density = component.Density;
				float& friction = component.Friction;
				float& restitution = component.Restitution;
				float& restitutionThreshold = component.RestitutionThreshold;

				ImGuiDraw::DrawVec2Control("Offset", component.Offset);
				ImGuiDraw::DrawVec1Control("Radius", component.Radius, 0.025f, 0.0001, 100000.f);

				ImGuiDraw::DrawVec1Control("Density", component.Density, 0.025f, 0.0001, 100000.f);
				ImGuiDraw::DrawVec1Control("Friction", component.Friction, 0.025f, 0.0001, 100000.f);
				ImGuiDraw::DrawVec1Control("Restitution", component.Restitution, 0.025f, 0.0001, 100000.f);
				ImGuiDraw::DrawVec1Control("Restitution Threshold", component.RestitutionThreshold, 0.025f, 0.0001, 100000.f);
			});

		DrawComponent<NativeScriptComponent>("Script", selection, [](auto& component) 
		{
			auto& nsc = component;
		});
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
	template<typename T, typename O>
	void SceneHierarchyPanel::DisplayAddComponentEntries(const std::string& entryOneName, const std::string& entryTwoName)
	{
		if (!m_SelectionContext.HasComponent<T>() && !m_SelectionContext.HasComponent<O>())
		{
			if (ImGui::MenuItem(entryOneName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
		if (!m_SelectionContext.HasComponent<O>() && !m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryTwoName.c_str()))
			{
				m_SelectionContext.AddComponent<O>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
}
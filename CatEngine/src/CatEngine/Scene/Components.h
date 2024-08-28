#pragma once

#include "CatEngine/Renderer/Texture.h"
#include "SoloAction.h"

#include "SceneCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CatEngine
{

	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name)
			: Name(name) {}
		virtual void ResetComponent() {};
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		virtual void ResetComponent() {};

	};

	struct LayerComponent
	{
		std::string Layer;

		LayerComponent() = default;
		LayerComponent(const LayerComponent&) = default;
		LayerComponent(const std::string& layer)
			: Layer(layer) {}

		virtual void ResetComponent() {};

	};
	
	struct TransformComponent
	{
		glm::vec3 Position = { 0.f, 0.f, 0.f };
		glm::vec3 Rotation = { 0.f, 0.f, 0.f };
		glm::vec3 Scale = { 1.f, 1.f, 1.f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position)
			: Position(position) {}

		glm::mat4& GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.f), Position)
				* rotation
				* glm::scale(glm::mat4(1.f), Scale);
		}

		virtual void ResetComponent()
		{
			Position = {};
			Rotation = {};
			Scale = { 1, 1, 1 };
		}
	};
	struct SpriteRendererComponent
	{
		
		glm::vec4 Color{ 1.f, 1.f, 1.f, 1.f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}

		virtual void ResetComponent()
		{
			Color = { 1.f, 1.f, 1.f, 1.f };
			Texture = nullptr;
			TilingFactor = 1.0f;
		}
	};

	struct CameraComponent
	{
		CatEngine::SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		virtual void ResetComponent()
		{
			if (Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				Camera.SetOrthographicFarClip(1.f);
				Camera.SetOrthographicNearClip(-1.f);
				Camera.SetOrthographicSize(10.f);
				Camera.SetOrthographic(10.f, -1.f, 1.f);
			}
			else
			{
				Camera.SetPerspectiveFarClip(10000.f);
				Camera.SetPerspectiveNearClip(.01f);
				Camera.SetPerspectiveVerticalFov(glm::radians(45.f));
				Camera.SetPerspective(glm::radians(45.f), 0.01f, 10000.f);
			}
		}
	};

	struct NativeScriptComponent
	{
		SoloAction* Instance = nullptr;


		SoloAction*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<SoloAction*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
		virtual void ResetComponent() {}
	};

}
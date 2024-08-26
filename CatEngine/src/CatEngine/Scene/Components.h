#pragma once

#include "CatEngine/Renderer/Texture.h"
#include "SoloAction.h"

#include "SceneCamera.h"

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

	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

	};

	struct LayerComponent
	{
		std::string Layer;

		LayerComponent() = default;
		LayerComponent(const LayerComponent&) = default;
		LayerComponent(const std::string& layer)
			: Layer(layer) {}

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
			glm::mat4 rotation = glm::rotate(glm::mat4(1.f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.f), Position)
				* rotation
				* glm::scale(glm::mat4(1.f), Scale);
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
	};

	struct CameraComponent
	{
		CatEngine::SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

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
	};

}
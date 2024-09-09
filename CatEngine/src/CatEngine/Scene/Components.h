#pragma once

#include "CatEngine/Renderer/Texture.h"
#include "SceneCamera.h"

#include "CatEngine/Core/UUID.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CatEngine
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;

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

	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name)
			: Name(name) {}
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

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.f, 1.f, 1.f, 1.f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;

		virtual void ResetComponent()
		{
			Color = { 1.f, 1.f, 1.f, 1.f };
			Thickness = 1.0f;
			Fade = 0.005f;
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

	class SoloAction;

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

	//---------------------------------------
	// 2D Components ========================
	//---------------------------------------

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Kinematic, Dynamic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;

		virtual void ResetComponent()
		{
			Type = BodyType::Static;
			FixedRotation = false;
		}
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		// TODO: Possibly make physics material
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

		virtual void ResetComponent()
		{
			Offset = { 0.0f, 0.0f };
			Size = { 0.5f, 0.5f };

			Density = 1.0f;
			Friction = 0.5f;
			Restitution = 0.0f;
			RestitutionThreshold = 0.5f;
		}
	};

	struct CircleCollider2DComponent 
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		// TODO: Possibly make physics material
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;

		virtual void ResetComponent()
		{
			Offset = { 0.0f, 0.0f };
			Radius = 0.5f;

			Density = 1.0f;
			Friction = 0.5f;
			Restitution = 0.0f;
			RestitutionThreshold = 0.5f;
		}
	};

}
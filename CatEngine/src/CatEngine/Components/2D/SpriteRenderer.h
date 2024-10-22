#pragma once

#include "CatEngine/Core/Core.h"
#include "CatEngine/Renderer/Texture.h"

namespace CatEngine
{
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
}

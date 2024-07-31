#pragma once

#include "CatEngine/Core/Camera/Camera.h"

#include "CatEngine/Core/Shader/Texture.h"


namespace CatEngine
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tileMultiplier = 1.f);
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tileMultiplier = 1.f);
	};
}


#pragma once

#include "CatEngine/Core/Camera/Camera.h"

#include "CatEngine/Core/Renderer/Texture.h"
#include "CatEngine/Core/Renderer/SubTexture2D.h"


namespace CatEngine
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void Flush();
		static void EndScene();

		// Primitives -- 2 component position
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
		{
			DrawQuad({ position.x ,position.y, 0.0f }, rotation, size, color);
		}

		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f)
		{
			DrawQuad({ position.x ,position.y, 0.0f }, rotation, size, texture, color, tilingFactor);
		}

		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, Ref<SubTexture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f)
		{
			DrawQuad({ position.x, position.y , 0.f }, rotation, size, texture, color, tilingFactor);
		}
		
		// Primitives -- 3 component position
		
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<SubTexture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);	


		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; };
			uint32_t GetTotalIndexCount() { return QuadCount * 4; }
		};

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void FlushAndReset();
		static void ResetData();

		static void IncrementData(const glm::vec3& position, float rotation, const glm::vec2& size, glm::vec4 color, Ref<Texture2D> texture = nullptr, Ref<SubTexture2D> subTexture = nullptr, float tilingFactor = 1.f);


	};
}


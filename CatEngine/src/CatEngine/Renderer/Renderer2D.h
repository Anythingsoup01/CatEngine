#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CatEngine/Camera/Camera.h"

#include "CatEngine/Renderer/Texture.h"
#include "CatEngine/Renderer/SubTexture2D.h"


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
		
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
		{
			DrawQuad(GetTransform(position, rotation, size), color);
		}
		
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f)
		{
			DrawQuad(GetTransform(position, rotation, size), texture, color, tilingFactor);
		}
		
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f)
		{
			DrawQuad(GetTransform(position, rotation, size), subTexture, color, tilingFactor);
		}

		// Primitives -- Transform Matrix
		
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));	
		static void DrawQuad(const glm::mat4& transform, Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.f), float tilingFactor = 1.f);
		static void DrawQuad(const glm::mat4& transform, Ref<SubTexture2D>& texture, const glm::vec4& color = glm::vec4(1.f), float tilingFactor = 1.f);


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
		static void IncrementData(const glm::mat4& transform, glm::vec4 color, const glm::vec2* textureCoords, float tilingFactor, float texIndex);
		static glm::mat4 GetTransform(const glm::vec3& position, float rotation, const glm::vec2 size) { return glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}


	};
}


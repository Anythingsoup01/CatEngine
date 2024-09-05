#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CatEngine/Camera/Camera.h"
#include "CatEngine/Camera/EditorCamera.h"

#include "CatEngine/Renderer/Texture.h"
#include "CatEngine/Renderer/SubTexture2D.h"

#include "CatEngine/Scene/Components.h"



namespace CatEngine
{
	class Renderer2D
	{
	public:

		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera); 
		static void BeginScene(const OrthographicCamera& camera); // TODO: Remove
		static void Flush();
		static void EndScene();

		// Primitives -- 2 component position
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
		{
			DrawQuad({ position.x ,position.y, 0.0f }, rotation, size, color);
		}

		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
		{
			DrawQuad({ position.x ,position.y, 0.0f }, rotation, size, texture, color, tilingFactor);
		}

		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, Ref<SubTexture2D>& texture, const glm::vec4& color, float tilingFactor)
		{
			DrawQuad({ position.x, position.y , 0.f }, rotation, size, texture, color, tilingFactor);
		}
		
		// Primitives -- 3 component position
		
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
		{
			DrawQuad(GetTransform(position, rotation, size), color, -1);
		}
		
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
		{
			DrawQuad(GetTransform(position, rotation, size), texture, color, tilingFactor, -1);
		}
		
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<SubTexture2D>& subTexture, const glm::vec4& color, float tilingFactor)
		{
			DrawQuad(GetTransform(position, rotation, size), subTexture, color, tilingFactor, -1);
		}

		// Primitives -- Transform Matrix
		
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID);
		static void DrawQuad(const glm::mat4& transform, Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor, int entityID);
		static void DrawQuad(const glm::mat4& transform, Ref<SubTexture2D>& texture, const glm::vec4& color, float tilingFactor, int entityID);


		// Sprites

		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

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
		static void NextBatch();
		static void StartBatch();
		static void IncrementData(const glm::mat4& transform, glm::vec4 color, const glm::vec2* textureCoords, float tilingFactor, float texIndex, int entityID);
		static glm::mat4 GetTransform(const glm::vec3& position, float rotation, const glm::vec2 size) { return glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}


	};
}



#pragma once

#include "CatEngine/Renderer/Texture.h"
#include "CatEngine/Renderer/SubTexture2D.h"

#include "CatEngine/Camera/Camera.h"
#include "CatEngine/Camera/EditorCamera.h"

#include "CatEngine/Components/Components.h"

#include "glm/glm.hpp"

namespace CatEngine {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera); // TODO: Remove
		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor);
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor);

		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, Ref<SubTexture2D>& subTexture, const glm::vec4& color, float tilingFactor);
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<SubTexture2D>& subTexture, const glm::vec4& color, float tilingFactor);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& texture, const glm::vec4& color, float tilingFactor, int entityID = -1);

		static void DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID = -1);

		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID = -1);

		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID = -1);
		static void DrawCircle(const glm::mat4& transform, CircleRendererComponent& crc, int entityID = -1);


		struct TextParams
		{
			glm::vec4 Color{ 1.0f };
			float Kerning = 0.0f;
			float LineSpacing = 0.0f;
		};

		static float GetLineWidth();
		static void SetLineThickness(float width);

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();

		static glm::mat4 GetTransform(const glm::vec3& position, float rotation, const glm::vec2 size) {
			return glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}
	};
}

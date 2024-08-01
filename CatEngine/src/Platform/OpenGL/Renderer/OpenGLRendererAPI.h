#pragma once
#include "CatEngine/Core/Renderer/RendererAPI.h"
namespace CatEngine {
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;

		void Clear(const glm::vec4 clearColor) override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	};
}

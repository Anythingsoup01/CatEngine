#pragma once
#include "CatEngine/Core/Renderer/Rendering/RendererAPI.h"
namespace CatEngine {
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;

		void Clear(const glm::vec4 clearColor) override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	};
}

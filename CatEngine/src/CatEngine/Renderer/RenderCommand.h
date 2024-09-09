#pragma once

#include "RendererAPI.h"

#include "CatEngine/Core/Core.h"

namespace CatEngine {
	
	class RenderCommand
	{
	public:
		static inline void Init()
		{
			s_RendererAPI->Init();
		}
		static inline void Clear(const glm::vec4 clearColor) {
			s_RendererAPI->Clear(clearColor);
		}
		static inline void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) {
			s_RendererAPI->DrawIndexed(vertexArray , indexCount);
		}
		static inline void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) {
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		static inline void SetLineThickness(float thickness)
		{
			s_RendererAPI->SetLineThickness(thickness);
		}

		static inline void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}

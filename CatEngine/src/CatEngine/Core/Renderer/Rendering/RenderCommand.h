#pragma once

#include "RendererAPI.h"

#include "CatEngine/Core/Core.h"

namespace CatEngine {
	
	class RenderCommand
	{
	public:
		static inline void Init()
		{
			s_API->Init();
		}
		static inline void Clear(const glm::vec4 clearColor) {
			s_API->Clear(clearColor);
		}
		static inline void DrawIndexed(const Ref<VertexArray>& vertexArray) {
			s_API->DrawIndexed(vertexArray);
		}

		static inline void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_API->SetViewport(x, y, width, height);
		}

	private:
		static RendererAPI* s_API;
	};
}

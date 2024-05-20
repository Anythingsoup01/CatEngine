#pragma once

#include "RendererAPI.h"

namespace CatEngine {
	
	class RenderCommand
	{
	public:
		static inline void Clear(const glm::vec4 clearColor) {
			s_API->Clear(clearColor);
		}
		static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
			s_API->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_API;
	};
}

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
	private:
		static RendererAPI* s_API;
	};
}

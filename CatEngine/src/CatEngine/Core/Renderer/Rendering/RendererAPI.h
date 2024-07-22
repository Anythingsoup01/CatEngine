#pragma once

#include "CatEngine/Core/Renderer/Buffers/VertexArray.h"

#include "glm/glm.hpp"

namespace CatEngine {
	
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
		};
	public:
		virtual void Init() = 0;

		virtual void Clear(const glm::vec4 clearColor) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}

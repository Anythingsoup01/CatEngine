#pragma once

#include "Buffers/Buffer.h"

namespace CatEngine {
	
	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1,
	};

	class Renderer {
	public:
		//inline RendererAPI SetRenderAPI();
		static inline RendererAPI GetCurrentAPI() { return s_RendererAPI; }

		void Clear();

		void Draw(const std::shared_ptr<IndexBuffer> indexBuffer);
	private:
		static RendererAPI s_RendererAPI;
	};

}




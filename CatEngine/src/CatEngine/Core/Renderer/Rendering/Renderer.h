#pragma once

#include "CatEngine/Core/Renderer/Buffers/Buffer.h"

#include "RenderCommand.h"

namespace CatEngine {

	class Renderer {
	public:
		//inline RendererAPI SetRenderAPI();
		static inline RendererAPI::API GetCurrentAPI() { return RendererAPI::GetAPI() ; }

		static void BeginScene(); // TODO Initiate default variables
		static void EndScene();
		
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		static void Draw(const std::shared_ptr<IndexBuffer> indexBuffer);
	};

}




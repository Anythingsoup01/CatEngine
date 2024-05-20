#pragma once
#include "CatEngine/Core/Renderer/Rendering/RendererAPI.h"
namespace CatEngine {
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Clear(const glm::vec4 clearColor) override;

		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

	};
}

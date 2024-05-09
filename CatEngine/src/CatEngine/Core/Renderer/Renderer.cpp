#include "cepch.h"
#include "Renderer.h"

#include <glad/glad.h> 

namespace CatEngine {
	RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void Renderer::Draw(const std::shared_ptr<IndexBuffer> indexBuffer)
	{
		glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
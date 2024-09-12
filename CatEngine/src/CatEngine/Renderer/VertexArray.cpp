#include "cepch.h"
#include "VertexArray.h"

#include "CatEngine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace CatEngine {
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:       CE_API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
		case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLVertexArray>();
		}
		CE_API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}

#include "cepch.h"
#include "VertexArray.h"

#include "CatEngine/Core/Renderer/Rendering/Renderer.h"

#include "Platform/OpenGL/Buffers/OpenGLVertexArray.h"

namespace CatEngine {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:       API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
		case RendererAPI::API::OpenGL:     return new OpenGLVertexArray();
		}
		API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
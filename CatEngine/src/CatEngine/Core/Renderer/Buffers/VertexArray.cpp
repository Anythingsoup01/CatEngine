#include "cepch.h"
#include "VertexArray.h"

#include "CatEngine/Core/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace CatEngine {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::None:       API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
		case RendererAPI::OpenGL:     return new OpenGLVertexArray();
		}
		API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
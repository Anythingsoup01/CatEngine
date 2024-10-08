#include "cepch.h"
#include "Buffer.h"

#include "CatEngine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#include "glad/glad.h"

namespace CatEngine {
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		CE_PROFILE_FUNCTION();

		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:       CE_API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
		case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLVertexBuffer>(size);
		}
		CE_API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		CE_PROFILE_FUNCTION();

		switch (Renderer::GetCurrentAPI())
		{
			case RendererAPI::API::None:       CE_API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
			case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}
		CE_API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
	Ref<IndexBuffer>  IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		CE_PROFILE_FUNCTION();

		switch (Renderer::GetCurrentAPI())
		{
			case RendererAPI::API::None:       CE_API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
			case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}
		CE_API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}

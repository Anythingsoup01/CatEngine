#include "cepch.h"
#include "Buffer.h"

#include "CatEngine/Core/Logging/Log.h"
#include "CatEngine/Core/Renderer/Rendering/Renderer.h"

#include "Platform/OpenGL/Buffers/OpenGLBuffer.h"

#include "glad/glad.h"

namespace CatEngine {
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
			case RendererAPI::API::None:       API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
			case RendererAPI::API::OpenGL:     return new OpenGLVertexBuffer(vertices, size);
		}
		API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
			case RendererAPI::API::None:       API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
			case RendererAPI::API::OpenGL:     return new OpenGLIndexBuffer(indices, size);
		}
		API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
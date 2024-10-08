#include "cepch.h"
#include "UniformBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace CatEngine
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		CE_PROFILE_FUNCTION();

		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:    CE_API_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		CE_API_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

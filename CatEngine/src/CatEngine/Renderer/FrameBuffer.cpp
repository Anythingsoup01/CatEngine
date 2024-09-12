#include "cepch.h"
#include "CatEngine/Renderer/Framebuffer.h"

#include "CatEngine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace CatEngine {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:    CE_API_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}

		CE_API_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

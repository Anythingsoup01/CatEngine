#include "cepch.h"
#include "GraphicsContext.h"

#include "CatEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"

namespace CatEngine {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:    API_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLGraphicsContext>(static_cast<GLFWwindow*>(window));
		}

		API_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
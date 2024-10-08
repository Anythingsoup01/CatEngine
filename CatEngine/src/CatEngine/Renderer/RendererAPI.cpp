#include "cepch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace CatEngine {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		CE_PROFILE_FUNCTION();

		switch (s_API)
		{
		case RendererAPI::API::None:    CE_API_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}

		CE_API_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

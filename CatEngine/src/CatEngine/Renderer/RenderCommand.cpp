#include "cepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace CatEngine {
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}

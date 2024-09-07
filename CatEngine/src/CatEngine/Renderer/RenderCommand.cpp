#include "cepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/Renderer/OpenGLRendererAPI.h"

namespace CatEngine {
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}

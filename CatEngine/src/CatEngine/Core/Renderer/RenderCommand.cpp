#include "cepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/Renderer/OpenGLRendererAPI.h"

namespace CatEngine {
	RendererAPI* RenderCommand::s_API = new OpenGLRendererAPI;
}

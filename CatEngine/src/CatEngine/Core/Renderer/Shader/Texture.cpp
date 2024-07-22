#include "cepch.h"
#include "Texture.h"

#include "CatEngine/Core/Renderer/Rendering/Renderer.h"
#include "Platform/OpenGL/Shader/OpenGLTexture.h"

namespace CatEngine
{
	Ref<Texture2D> CatEngine::Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:       API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
		case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLTexture2D>(path);
		}
		API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

}


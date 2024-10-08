#include "cepch.h"
#include "Texture.h"

#include "CatEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace CatEngine
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		CE_PROFILE_FUNCTION();

		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:       CE_API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
		case RendererAPI::API::OpenGL:     return CreateRef<OpenGLTexture2D>(width, height);
		}
		CE_API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
	Ref<Texture2D> CatEngine::Texture2D::Create(const std::string& path)
	{
		CE_PROFILE_FUNCTION();

		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:       CE_API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
		case RendererAPI::API::OpenGL:     return CreateRef<OpenGLTexture2D>(path);
		}
		CE_API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

}

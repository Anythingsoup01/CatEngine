#include "cepch.h"
#include "FrameBuffer.h"

#include "CatEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/Renderer/OpenGLFrameBuffer.h"

namespace CatEngine
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:       API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
		case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLFrameBuffer>(spec);
		}
		API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}

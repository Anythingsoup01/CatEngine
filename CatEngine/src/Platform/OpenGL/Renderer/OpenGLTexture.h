#pragma once

#include <string>

#include <glad/glad.h>
#include "CatEngine/Renderer/Texture.h"

namespace CatEngine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string path);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; };
		virtual uint32_t GetHeight() const override { return m_Height; };

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot) const override;
		virtual void Unbind() const override;


		virtual bool operator==(const Texture& other) const override;

	private:
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_Format;
	};
}


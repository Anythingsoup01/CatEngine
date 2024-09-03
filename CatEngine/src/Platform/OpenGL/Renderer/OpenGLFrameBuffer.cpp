#include "cepch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace CatEngine
{
	static const uint32_t s_MaxFrameBufferSize = 8192;
	namespace Utils
	{

		static GLenum TextureTarget(bool multiSample)
		{
			return multiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multiSample, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multiSample), count, outID);
		}

		static void BindTexture(bool multiSample, uint32_t id)
		{
			glBindTexture(TextureTarget(multiSample), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multiSample = samples > 1;
			if (multiSample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);

			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multiSample), id, 0);

		}
		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multiSample = samples > 1;
			if (multiSample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);

			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multiSample), id, 0);

		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
				case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
			}
			return false;
		}

		static GLenum CatEngineTextureFormatToGL(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case CatEngine::FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
				break;
			case CatEngine::FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
				break;
			case CatEngine::FrameBufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
				break;
			}
			API_ASSERT(false, "Format not supported!");
			return 0;
		}

		static GLenum GLDataType(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case CatEngine::FrameBufferTextureFormat::RGBA8: return GL_FLOAT;
				break;
			case CatEngine::FrameBufferTextureFormat::RED_INTEGER: return GL_INT;
				break;
			case CatEngine::FrameBufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
				break;
			}
			API_ASSERT(false, "Format not supported!");
			return 0;
		}
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecs.emplace_back(spec);
			else
				m_DepthAttachmentSpec = spec;
		}
		Invalidate();
	}
	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		CE_PROFILE_FUNCTION();
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}
	void OpenGLFrameBuffer::Invalidate()
	{
		CE_PROFILE_FUNCTION();
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multiSample = m_Specification.Samples > 1;

		// Attachments

		if (m_ColorAttachmentSpecs.size())
		{
			m_ColorAttachments.resize((size_t)m_ColorAttachmentSpecs.size());
			Utils::CreateTextures(multiSample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachmentSpecs.size(); i++)
			{
				Utils::BindTexture(multiSample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecs[i].TextureFormat)
				{
					case FrameBufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, (int)i);
						break;
					case FrameBufferTextureFormat::RED_INTEGER:
						Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, (int)i);
						break;
				}
			}
		}

		if (m_DepthAttachmentSpec.TextureFormat != FrameBufferTextureFormat::None)
		{
			Utils::CreateTextures(multiSample, &m_DepthAttachment, 1);
			Utils::BindTexture(multiSample, m_DepthAttachment);
			switch (m_DepthAttachmentSpec.TextureFormat)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			API_ASSERT(m_ColorAttachments.size() <= 4, "");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only Depth pass
			glDrawBuffer(GL_NONE);
		}

		API_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "GL_FRAMEBUFFER Incomplete!")

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Bind()
	{
		CE_PROFILE_FUNCTION();
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

		// ClearColorAttachment<int>(1, value); 

		int value = -1;
		glClearTexImage(m_ColorAttachments[1], 0, GL_RED_INTEGER, GL_INT, &value);
	}
	void OpenGLFrameBuffer::Unbind()
	{
		CE_PROFILE_FUNCTION();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::SetSize(uint32_t width, uint32_t height)
	{
		CE_PROFILE_FUNCTION();
		if (width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
		{
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}
	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		API_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of range!");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}
	void OpenGLFrameBuffer::ClearColorAttachmentI(uint32_t attachmentIndex, int value)
	{
		API_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of range!");
		auto& spec = m_ColorAttachmentSpecs[attachmentIndex];

		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::CatEngineTextureFormatToGL(spec.TextureFormat), GL_INT, &value);

	}
}
#include "cepch.h"
#include "OpenGLVertexArray.h"



#include "glad/glad.h"

namespace CatEngine {

	static GLenum ShaderDataTypeToGLBaseType(ShaderDataType type) {
		switch (type)
		{
			case CatEngine::ShaderDataType::Vec:  return GL_FLOAT;
			case CatEngine::ShaderDataType::Vec2: return GL_FLOAT;
			case CatEngine::ShaderDataType::Vec3: return GL_FLOAT;
			case CatEngine::ShaderDataType::Vec4: return GL_FLOAT;
			case CatEngine::ShaderDataType::Mat3: return GL_FLOAT;
			case CatEngine::ShaderDataType::Mat4: return GL_FLOAT;
			case CatEngine::ShaderDataType::Int:  return GL_INT;
			case CatEngine::ShaderDataType::Int2: return GL_INT;
			case CatEngine::ShaderDataType::Int3: return GL_INT;
			case CatEngine::ShaderDataType::Int4: return GL_INT;
			case CatEngine::ShaderDataType::Bool: return GL_BOOL;
		}
		CE_API_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
	}
	void OpenGLVertexArray::Bind() const 
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		CE_PROFILE_FUNCTION();
		CE_API_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer Has no layout!!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Vec:
			case ShaderDataType::Vec2:
			case ShaderDataType::Vec3:
			case ShaderDataType::Vec4:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)(intptr_t)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)(intptr_t)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						ShaderDataTypeToGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(intptr_t)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			default:
				CE_API_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		m_VertexBuffer.push_back(vertexBuffer);
	}	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}

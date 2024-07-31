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
		API_ASSERT(false, "Unknown ShaderDataType!");
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
		API_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer Has no layout!!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(element.Type),
				ShaderDataTypeToGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffer.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
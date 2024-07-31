#include "cepch.h"
#include "Renderer2D.h"

#include "CatEngine/Core/Core.h"

#include <glm/gtc/matrix_transform.hpp>

#include "CatEngine/Core/Buffers/VertexArray.h"
#include "CatEngine/Core/Shader/Shader.h"
#include "CatEngine/Core/Renderer/RenderCommand.h"

#include "CatEngine/Core/Timer.h"


namespace CatEngine
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_TextureShader;
		Ref<Texture2D> m_DefaultTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		CE_PROFILE_FUNCTION();
		s_Data = new Renderer2DStorage();
		s_Data->m_VertexArray = CatEngine::VertexArray::Create();

		// Vertex Buffer
		float vertices[5 * 4]{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 0
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // 1
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 2
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // 3
		};

		Ref<CatEngine::VertexBuffer> squareBuffer;
		squareBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		squareBuffer->SetLayout
		({
			{ ShaderDataType::Vec3, "a_Position", false },
			{ ShaderDataType::Vec2, "a_TexCoord", false }
			});
		s_Data->m_VertexArray->AddVertexBuffer(squareBuffer);

		// Index Buffer
		unsigned int indices[]{ 0, 1, 2, 1, 3, 2 };

		Ref<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		s_Data->m_VertexArray->SetIndexBuffer(squareIndexBuffer);
		
		s_Data->m_DefaultTexture = Texture2D::Create(1, 1);
		uint32_t defaultTextureData = 0xffffffff;
		s_Data->m_DefaultTexture->SetData(&defaultTextureData, sizeof(uint32_t));

		s_Data->m_TextureShader = Shader::Create("assets/shaders/TextureShader.glsl");
		s_Data->m_TextureShader->Bind();
		s_Data->m_TextureShader->SetInt("u_Texture", 0);
	}
	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		CE_PROFILE_FUNCTION();
		s_Data->m_TextureShader->Bind();
		s_Data->m_TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
	}
	void Renderer2D::EndScene()
	{

	}
	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x ,position.y, 0.0f }, rotation, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		CE_PROFILE_FUNCTION();
		s_Data->m_TextureShader->SetVec4("u_Color", color);
		s_Data->m_DefaultTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->m_TextureShader->SetMat4("u_ModelMatrix", transform);

		s_Data->m_VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->m_VertexArray);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color, float tileMultiplier)
	{
		DrawQuad({ position.x ,position.y, 0.0f }, rotation, size, texture, color, tileMultiplier);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color, float tileMultiplier)
	{
		CE_PROFILE_FUNCTION();
		s_Data->m_TextureShader->SetVec4("u_Color", color);
		s_Data->m_TextureShader->SetVec1("u_TexTile", tileMultiplier);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->m_TextureShader->SetMat4("u_ModelMatrix", transform);

		texture->Bind();

		s_Data->m_VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->m_VertexArray);
	}
}
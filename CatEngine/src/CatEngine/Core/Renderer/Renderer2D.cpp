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
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};
	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t m_MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> DefaultTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, m_MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = DefaultTexture
	};

	static Renderer2DData s_Data;


	void Renderer2D::Init()
	{
		CE_PROFILE_FUNCTION();
		s_Data.QuadVertexArray = CatEngine::VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout
		({
			{ ShaderDataType::Vec3, "a_Position" },
			{ ShaderDataType::Vec4, "a_Color" },
			{ ShaderDataType::Vec2, "a_TexCoord" },
			{ ShaderDataType::Vec, "a_TexID" },
			{ ShaderDataType::Vec, "a_TilingFactor" }
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);


		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];


		// Index Buffer
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];


		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;
		
		s_Data.DefaultTexture = Texture2D::Create(1, 1);
		uint32_t defaultTextureData = 0xffffffff;
		s_Data.DefaultTexture->SetData(&defaultTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.m_MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.m_MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_Data.TextureShader = Shader::Create("assets/shaders/TextureShader.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Texture", samplers, s_Data.m_MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.DefaultTexture;
	}
	void Renderer2D::Shutdown()
	{

	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		CE_PROFILE_FUNCTION();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
		
	}
	void Renderer2D::Flush()
	{
		// Bind Textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}


		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}
	void Renderer2D::EndScene()
	{
		CE_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x ,position.y, 0.0f }, rotation, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		CE_PROFILE_FUNCTION();

		const float texIndex = 0.f;
		const float tilingFactor = 0.f;

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
#if OLD_PATH
		s_Data.m_TextureShader->SetVec1("u_TexTile", 1.0f);
		s_Data.m_DefaultTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.m_TextureShader->SetMat4("u_ModelMatrix", transform);

		s_Data.m_QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray);
#endif
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color, float tileMultiplier)
	{
		DrawQuad({ position.x ,position.y, 0.0f }, rotation, size, texture, color, tileMultiplier);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		CE_PROFILE_FUNCTION();

		constexpr glm::vec4 defaultColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = defaultColor * color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.f };
		s_Data.QuadVertexBufferPtr->Color = defaultColor * color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.f };
		s_Data.QuadVertexBufferPtr->Color = defaultColor * color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.f };
		s_Data.QuadVertexBufferPtr->Color = defaultColor * color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

#if OLD_PATH
		s_Data.m_TextureShader->SetVec4("u_Color", color);
		s_Data.m_TextureShader->SetVec1("u_TexTile", tileMultiplier);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.m_TextureShader->SetMat4("u_ModelMatrix", transform);

		texture->Bind();

		s_Data.m_QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray);
#endif
	}
}
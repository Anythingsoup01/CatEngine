#include "cepch.h"
#include "Renderer2D.h"

#include "CatEngine/Core/Core.h"

#include "CatEngine/Renderer/VertexArray.h"
#include "CatEngine/Renderer/Shader.h"
#include "CatEngine/Renderer/RenderCommand.h"

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
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;

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



		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;

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
			{ ShaderDataType::Vec, "a_TexIndex" },
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


		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	}
	void Renderer2D::Shutdown()
	{

	}
	void Renderer2D::ResetData()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}


	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		CE_PROFILE_FUNCTION();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());


		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;

		ResetData();

		
	}
	void Renderer2D::Flush()
	{

		CE_PROFILE_FUNCTION();

		// Bind Textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}


		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}


	void Renderer2D::FlushAndReset()
	{
		EndScene();
		
		ResetData();
	}

	void Renderer2D::EndScene()
	{
		CE_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		constexpr glm::vec4 d_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec2 textureCoords[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
		float texIndex = 0.f;
		const float tilingFactor = 1.f;


		IncrementData(transform, color, textureCoords, tilingFactor, texIndex);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		glm::vec2 textureCoords[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
		float texIndex = 0.f;

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

		IncrementData(transform, color, textureCoords, tilingFactor, texIndex);

	}


	void Renderer2D::DrawQuad(const glm::mat4& transform, Ref<SubTexture2D>& subTexture, const glm::vec4& color, float tilingFactor)
	{
		Ref<Texture2D> texture;
		const glm::vec2* textureCoords;
		float texIndex = 0.f;

		textureCoords = subTexture->GetTexCoord();
		texture = subTexture->GetTexture();
		
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

		IncrementData(transform, color, textureCoords, tilingFactor, texIndex);
	}
	
	void Renderer2D::ResetStats() { memset(&s_Data.Stats, 0, sizeof(Renderer2D::Statistics)); }
	Renderer2D::Statistics Renderer2D::GetStats() { return s_Data.Stats; }

	void Renderer2D::IncrementData(const glm::mat4& transform, glm::vec4 color, const glm::vec2* textureCoords, float tilingFactor, float texIndex)
	{
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}


		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}
}
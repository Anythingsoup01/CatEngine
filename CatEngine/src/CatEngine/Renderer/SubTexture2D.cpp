#include "cepch.h"
#include "SubTexture2D.h"

namespace CatEngine
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}
	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, float x, float y, float cellWidth, float cellHeight, float spriteWidth, float spriteHeight)
	{
		return CreateFromCoords(texture, { x, y }, {cellWidth, cellHeight}, {spriteWidth, spriteHeight});
	}
	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		glm::vec2 min = { (cellSize.x * coords.x) / texture->GetWidth(), (cellSize.y * coords.y) / texture->GetHeight() };
		glm::vec2 max = { (cellSize.x * (coords.x + spriteSize.x)) / texture->GetWidth(), (cellSize.y * (coords.y + spriteSize.y)) / texture->GetHeight() };
		Ref<SubTexture2D> result = CreateRef<SubTexture2D>(texture, min, max);

		return result;
	}
}
#pragma once
#include "CatEngine.h"

class Sandbox2D : public CatEngine::Layer
{
public:
	Sandbox2D();

	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(CatEngine::Time time) override;
	virtual void OnImGuiDraw() override;
	virtual void OnEvent(CatEngine::Event& e) override;
private:

	CatEngine::OrthographicCameraController m_CameraController;

<<<<<<< HEAD
	// Temp
	CatEngine::Ref<CatEngine::VertexArray> m_QuadVertexArray;
	CatEngine::Ref<CatEngine::Shader> m_Shader;

	glm::vec4 m_BoxOne = {1,1,1,1};
	glm::vec4 m_BoxTwo = {1,1,1,1};
	float m_TexTile = 1.f;

=======
	glm::vec4 m_BoxOne = {1,1,1,1};
	 int m_Grid[2] = {1, 1};

>>>>>>> e0fb026 (Improved Batch Renderer)
	CatEngine::Ref<CatEngine::Texture2D> m_CatTexture;
	CatEngine::Ref<CatEngine::Texture2D> m_CheckeredTexture;

	float rotation = 0.0f;
};
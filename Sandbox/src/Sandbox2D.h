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

	// Temp
	CatEngine::Ref<CatEngine::VertexArray> m_QuadVertexArray;
	CatEngine::Ref<CatEngine::Shader> m_Shader;

	glm::vec4 m_BoxOne = {1,1,1,1};
	glm::vec4 m_BoxTwo = {1,1,1,1};
	glm::vec1 m_TexTile = glm::vec1(1.f);

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	CatEngine::Ref<CatEngine::Texture2D> m_Texture;

};
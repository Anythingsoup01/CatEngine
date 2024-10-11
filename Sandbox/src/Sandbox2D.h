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

	float m_Width = 10.f, m_Height = 10.f;

	CatEngine::Ref<CatEngine::Texture2D> m_CheckeredTexture;

	CatEngine::OrthographicCamera m_SceneOrthoCam;

};

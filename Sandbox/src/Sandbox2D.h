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
	CatEngine::Ref<CatEngine::VertexArray> m_VertexArray;
	CatEngine::ShaderLibrary m_ShaderLibrary;

	glm::vec3 m_SquareTransform = { 0,0,0 };

	int m_GridSize[2] = { 1, 1 };

};
#include <iostream>

#include "CatEngine.h"


class ExampleLayer : public CatEngine::Layer {
public:
	ExampleLayer()
		:Layer("Example") {}
	
	void OnUpdate() override 
	{
		DEBUG_INFO("ExampleLayer::Update");
	}

	void OnEvent(CatEngine::Events& event) override 
	{
		DEBUG_TRACE("{0}", event.ToString());
	}
};

class Sandbox : public CatEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new CatEngine::ImGuiLayer());
	}
	~Sandbox() {

	}
};

CatEngine::Application* CatEngine::CreateApplication() {
	return new Sandbox();
}
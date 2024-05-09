#include <iostream>

#include "CatEngine.h"

#include "imgui.h"

class ExampleLayer : public CatEngine::Layer {
public:
	ExampleLayer()
		:Layer("Example") {}
	
	void OnUpdate() override 
	{
		//CLI_INFO("ExampleLayer::Update");
	}

	void OnEvent(CatEngine::Events& event) override 
	{
		if (event.GetEventType() == CatEngine::EventType::KeyPressed) {
			CatEngine::KeyPressedEvent& e = (CatEngine::KeyPressedEvent&)event;
			API_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public CatEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};

CatEngine::Application* CatEngine::CreateApplication() {
	return new Sandbox();
}
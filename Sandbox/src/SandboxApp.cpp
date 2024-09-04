#include "CatEngine.h"
#include "CatEngine/Core/EntryPoint.h"

#include "Sandbox2D.h"

class Sandbox : public CatEngine::Application {
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {

	}
};

CatEngine::Application* CatEngine::CreateApplication(AppllicationCommandLineArgs args) {
	return new Sandbox();
}
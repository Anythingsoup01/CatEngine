#include "CatEngine.h"
#include "CatEngine/Core/EntryPoint.h"

#include "Sandbox2D.h"

class Sandbox : public CatEngine::Application {
public:
	Sandbox(CatEngine::ApplicationSpecification& specification)
		:Application(specification)
	{
		PushLayer(new Sandbox2D());
	}
};

CatEngine::Application* CatEngine::CreateApplication(ApplicationCommandLineArgs args) {
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../CatEditor";
	spec.CommandlineArgs = args;
	return new Sandbox(spec);
}

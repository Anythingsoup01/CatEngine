#include "CatEngine.h"
#include "CatEngine/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace CatEngine
{

	class CatEditor : public Application
	{
	public:
		CatEditor(const ApplicationSpecification& specification)
			: Application(specification)
		{
			PushLayer(new EditorLayer());
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		ApplicationSpecification spec;
		spec.Name = "CatEditor";
		spec.CommandlineArgs = args;
		return new CatEditor(spec);
	}
}

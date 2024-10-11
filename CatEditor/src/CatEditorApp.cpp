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
		~CatEditor() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		ApplicationSpecification spec;
		spec.Name = "CatEditor";
		spec.WorkingDirectory = "./";
		spec.CommandlineArgs = args;
		return new CatEditor(spec);
	}
}

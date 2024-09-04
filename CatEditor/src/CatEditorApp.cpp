#include "CatEngine.h"
#include "CatEngine/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace CatEngine
{

	class CatEditor : public Application
	{
	public:
		CatEditor()
			: Application("CatEditor")
		{
			PushLayer(new EditorLayer());
		}
		~CatEditor() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		return new CatEditor();
	}
}

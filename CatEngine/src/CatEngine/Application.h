#pragma once

#include "Core.h"



namespace CatEngine {

	class __declspec(dllexport) Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//Defined in client
	Application* CreateApplication();
}

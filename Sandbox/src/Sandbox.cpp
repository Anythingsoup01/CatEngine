#include "CatEngine.h"
#include <iostream>


class Sandbox : public CatEngine::Application {
public:
	Sandbox() 
	{

	}
	~Sandbox() 
	{

	}
};

CatEngine::Application* CatEngine::CreateApplication() {
	return new Sandbox();
}
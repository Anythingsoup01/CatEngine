#include "CatEngine.h"
#include <iostream>
class Sandbox : public CatEngine::Application {
public:
	Sandbox() 
	{
		std::cout << CE_API_STATUS << std::endl;
	}
	~Sandbox() 
	{
	
	}
};

CatEngine::Application* CatEngine::CreateApplication() {
	return new Sandbox();
}
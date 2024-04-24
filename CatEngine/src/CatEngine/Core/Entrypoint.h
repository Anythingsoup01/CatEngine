#pragma once

#ifdef CE_PLATFORM_WINDOWS

extern CatEngine::Application* CatEngine::CreateApplication();

int main(int argc, char** argv) {

	CatEngine::Log::Init();

	auto app = CatEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif // CE_PLATFORM_WINDOWS


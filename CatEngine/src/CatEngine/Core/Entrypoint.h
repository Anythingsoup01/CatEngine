#pragma once

#ifdef CE_PLATFORM_WINDOWS

extern CatEngine::Application* CatEngine::CreateApplication(AppllicationCommandLineArgs args);

int main(int argc, char** argv) {
	CatEngine::Log::Init();

	CE_PROFILE_BEGIN_SESSION("Startup", "CatEngine-Startup.json");
	auto app = CatEngine::CreateApplication({argc, argv});
	CE_PROFILE_END_SESSION();

	CE_PROFILE_BEGIN_SESSION("Runtime", "CatEngine-Runtime.json");
	app->Run();
	CE_PROFILE_END_SESSION();

	CE_PROFILE_BEGIN_SESSION("Shutdown", "CatEngine-Shutdown.json");
	delete(app);
	CE_PROFILE_END_SESSION();
}

#endif // CE_PLATFORM_WINDOWS


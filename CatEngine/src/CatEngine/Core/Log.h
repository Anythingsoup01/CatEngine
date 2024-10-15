#pragma once
#include "cepch.h"

#include "CatEngine/Core/Core.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) Log::GLClearError();x;ASSERT(Log::GLLog(#x, __FILE__, __LINE__));



namespace CatEngine {
	class Log
	{
	public:
		static void GLClearError();

		static bool GLLog(const char* function, const char* file, int line);

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetAPILogger() { return s_APILogger; }
		inline static std::shared_ptr<spdlog::logger>& GetCLILogger() { return s_CLILogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_APILogger;
		static std::shared_ptr<spdlog::logger> s_CLILogger;
	};
}

#ifdef CE_DEBUG

// Core Log Macros
#define CE_API_CRITICAL(...)             ::CatEngine::Log::GetAPILogger()->critical(__VA_ARGS__)
#define CE_API_ERROR(...)                ::CatEngine::Log::GetAPILogger()->error(__VA_ARGS__)
#define CE_API_WARN(...)                 ::CatEngine::Log::GetAPILogger()->warn(__VA_ARGS__)
#define CE_API_INFO(...)                 ::CatEngine::Log::GetAPILogger()->info(__VA_ARGS__)
#define CE_API_TRACE(...)                ::CatEngine::Log::GetAPILogger()->trace(__VA_ARGS__)

// Client Log Macros
#define CE_CLI_CRITICAL(...)             ::CatEngine::Log::GetCLILogger()->critical(__VA_ARGS__)
#define CE_CLI_ERROR(...)                ::CatEngine::Log::GetCLILogger()->error(__VA_ARGS__)
#define CE_CLI_WARN(...)                 ::CatEngine::Log::GetCLILogger()->warn(__VA_ARGS__)
#define CE_CLI_INFO(...)                 ::CatEngine::Log::GetCLILogger()->info(__VA_ARGS__)
#define CE_CLI_TRACE(...)                ::CatEngine::Log::GetCLILogger()->trace(__VA_ARGS__)
#elif CE_RELEASE || CE_DIST
// Core Log Macros
#define CE_API_CRITICAL(...)
#define CE_API_ERROR(...)
#define CE_API_WARN(...)
#define CE_API_INFO(...)
#define CE_API_TRACE(...)

// Client Log Macros
#define CE_CLI_CRITICAL(...)             ::CatEngine::Log::GetCLILogger()->critical(__VA_ARGS__)
#define CE_CLI_ERROR(...)                ::CatEngine::Log::GetCLILogger()->error(__VA_ARGS__)
#define CE_CLI_WARN(...)                 ::CatEngine::Log::GetCLILogger()->warn(__VA_ARGS__)
#define CE_CLI_INFO(...)                 ::CatEngine::Log::GetCLILogger()->info(__VA_ARGS__)
#define CE_CLI_TRACE(...)                ::CatEngine::Log::GetCLILogger()->trace(__VA_ARGS__)

#endif // CE_DEBUG


#pragma once
#include "cepch.h"

#include "Core.h"
#include "spdlog/spdlog.h"

namespace CatEngine {
	class CE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

#ifdef CE_DEBUG
// Core Log Macros
#define CORE_FATAL(...) ::CatEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CORE_ERROR(...) ::CatEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_WARN(...)  ::CatEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_INFO(...)  ::CatEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_TRACE(...) ::CatEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_ASSERT(x, ...)if (!(x)) { ::CatEngine::Log::GetCoreLogger()->critical(__VA_ARGS__); __debugbreak(); }
// Client Log Macros
#define DEBUG_FATAL(...) ::CatEngine::Log::GetClientLogger()->critical(__VA_ARGS__)
#define DEBUG_ERROR(...) ::CatEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define DEBUG_WARN(...)  ::CatEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DEBUG_INFO(...)  ::CatEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define DEBUG_TRACE(...) ::CatEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#elif CE_RELEASE
// Core Log Macros
#define CORE_FATAL(...)
#define CORE_ERROR(...)
#define CORE_WARN(...)
#define CORE_INFO(...)
#define CORE_TRACE(...)
// Client Log Macros
#define DEBUG_FATAL(...) ::CatEngine::Log::GetClientLogger()->critical(__VA_ARGS__)
#define DEBUG_ERROR(...) ::CatEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define DEBUG_WARN(...)  ::CatEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DEBUG_INFO(...)  ::CatEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define DEBUG_TRACE(...) ::CatEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#elif CE_DIST
// Core Log Macros
#define CORE_FATAL(...)
#define CORE_ERROR(...)
#define CORE_WARN(...)
#define CORE_INFO(...)
#define CORE_TRACE(...)
// Client Log Macros
#define DEBUG_FATAL(...)
#define DEBUG_ERROR(...)
#define DEBUG_WARN(...)
#define DEBUG_INFO(...)
#define DEBUG_TRACE(...)
#endif
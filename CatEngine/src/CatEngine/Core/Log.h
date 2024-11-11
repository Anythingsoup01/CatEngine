#pragma once
#include "cepch.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) Logger::GLClearError();x;ASSERT(Logger::GLLog(#x, __FILE__, __LINE__));



namespace CatEngine {
	class Logger
	{
	public:
		static void GLClearError();

		static bool GLLog(const char* function, const char* file, int line);

		static void Init();

		inline static Ref<spdlog::logger>& GetAPILogger() { return s_APILogger; }
		inline static Ref<spdlog::logger>& GetCLILogger() { return s_CLILogger; }
	private:
		static Ref<spdlog::logger> s_APILogger;
		static Ref<spdlog::logger> s_CLILogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternio)
{
	return os << glm::to_string(quaternio);
}



#ifdef CE_DEBUG

// Core Log Macros
#define CE_API_CRITICAL(...)             ::CatEngine::Logger::GetAPILogger()->critical(__VA_ARGS__)
#define CE_API_ERROR(...)                ::CatEngine::Logger::GetAPILogger()->error(__VA_ARGS__)
#define CE_API_WARN(...)                 ::CatEngine::Logger::GetAPILogger()->warn(__VA_ARGS__)
#define CE_API_INFO(...)                 ::CatEngine::Logger::GetAPILogger()->info(__VA_ARGS__)
#define CE_API_TRACE(...)                ::CatEngine::Logger::GetAPILogger()->trace(__VA_ARGS__)

// Client Log Macros
#define CE_CLI_CRITICAL(...)             ::CatEngine::Logger::GetCLILogger()->critical(__VA_ARGS__)
#define CE_CLI_ERROR(...)                ::CatEngine::Logger::GetCLILogger()->error(__VA_ARGS__)
#define CE_CLI_WARN(...)                 ::CatEngine::Logger::GetCLILogger()->warn(__VA_ARGS__)
#define CE_CLI_INFO(...)                 ::CatEngine::Logger::GetCLILogger()->info(__VA_ARGS__)
#define CE_CLI_TRACE(...)                ::CatEngine::Logger::GetCLILogger()->trace(__VA_ARGS__)
#elif CE_RELEASE || CE_DIST
// Core Log Macros
#define CE_API_CRITICAL(...)
#define CE_API_ERROR(...)
#define CE_API_WARN(...)
#define CE_API_INFO(...)
#define CE_API_TRACE(...)

// Client Log Macros
#define CE_CLI_CRITICAL(...)             ::CatEngine::Logger::GetCLILogger()->critical(__VA_ARGS__)
#define CE_CLI_ERROR(...)                ::CatEngine::Logger::GetCLILogger()->error(__VA_ARGS__)
#define CE_CLI_WARN(...)                 ::CatEngine::Logger::GetCLILogger()->warn(__VA_ARGS__)
#define CE_CLI_INFO(...)                 ::CatEngine::Logger::GetCLILogger()->info(__VA_ARGS__)
#define CE_CLI_TRACE(...)                ::CatEngine::Logger::GetCLILogger()->trace(__VA_ARGS__)

#endif // CE_DEBUG


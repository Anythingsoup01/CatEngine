#include "cepch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

#include "glad/glad.h"

namespace CatEngine {

    void Log::GLClearError() {
        while (glGetError() != GL_NO_ERROR);
    }

    bool Log::GLLog(const char* function, const char* file, int line)
    {
        if (GLenum error = glGetError()) {
            if (GLenum error = GL_INVALID_ENUM) {
                std::cout << "[OpenGL Error] (Invalid Enum): " << function << " " << file << " (" << line << ")" << std::endl;
                return false;
            }
            else if (GLenum error = GL_INVALID_VALUE) {
                std::cout << "[OpenGL Error] (Invalid Value): " << function << " " << file << " (" << line << ")" << std::endl;
                return false;
            }
            else if (GLenum error = GL_INVALID_OPERATION) {
                std::cout << "[OpenGL Error] (Invalid Operation): " << function << " " << file << " (" << line << ")" << std::endl;
                return false;
            }
            else if (GLenum error = GL_STACK_OVERFLOW) {
                std::cout << "[OpenGL Error] (Stack Overflow): " << function << " " << file << " (" << line << ")" << std::endl;
                return false;
            }
            else if (GLenum error = GL_INVALID_FRAMEBUFFER_OPERATION) {
                std::cout << "[OpenGL Error] (Invalid Framebuffer Operation): " << function << " " << file << " (" << line << ")" << std::endl;
                return false;
            }
            else if (GLenum error = GL_CONTEXT_LOST) {
                std::cout << "[OpenGL Error] (Context Lost): " << function << " " << file << " (" << line << ")" << std::endl;
                return false;
            }
        }
        return true;
    }

	std::shared_ptr<spdlog::logger> Log::s_APILogger;
	std::shared_ptr<spdlog::logger> Log::s_CLILogger;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_APILogger = spdlog::stdout_color_mt("CEAPI");
		s_APILogger->set_level(spdlog::level::trace);

		s_CLILogger = spdlog::stdout_color_mt("CLI");
		s_CLILogger->set_level(spdlog::level::trace);
	}



}

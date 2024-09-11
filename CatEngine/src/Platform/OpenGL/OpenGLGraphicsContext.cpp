#include "cepch.h"

#include "OpenGLGraphicsContext.h"
#include "CatEngine/Core/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace CatEngine {
	OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* windowHandle)
		: m_Window(windowHandle)
	{
		API_ASSERT(windowHandle, "Window Handle is null!");
	}

	void OpenGLGraphicsContext::Init()
	{
		CE_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		API_ASSERT(status, "Failed to load GLAD");

#ifdef CE_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		API_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "CatEngine requires at least OpenGL version 4.5!");
#endif
	}

	void OpenGLGraphicsContext::SwapBuffers()
	{
		CE_PROFILE_FUNCTION();
		glfwSwapBuffers(m_Window);
	}
}
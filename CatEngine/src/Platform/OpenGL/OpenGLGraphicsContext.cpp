#include "cepch.h"
#include "OpenGLGraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
	}

	void OpenGLGraphicsContext::SwapBuffers()
	{
		CE_PROFILE_FUNCTION();
		glfwSwapBuffers(m_Window);
	}
}
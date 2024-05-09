#include "cepch.h"
#include "OpenGLGraphicsContext.h"

#include "CatEngine/Core/Logging/Log.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace CatEngine {
	OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		API_ASSERT(windowHandle, "Window Handle is null!");
	}

	void OpenGLGraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		API_ASSERT(status, "Failed to load GLAD");
	}

	void OpenGLGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
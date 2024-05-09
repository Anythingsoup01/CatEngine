#pragma once
#include "CatEngine/Core/Renderer/GraphicsAPIContext.h"

struct GLFWwindow;

namespace CatEngine {

	class OpenGLGraphicsContext : public GraphicsAPIContext {
	public:
		OpenGLGraphicsContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}
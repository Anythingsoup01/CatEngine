#pragma once
#include "CatEngine/Core/GraphicsContext.h"

struct GLFWwindow;

namespace CatEngine {

	class OpenGLGraphicsContext : public GraphicsContext {
	public:
		OpenGLGraphicsContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_Window;
	};
}
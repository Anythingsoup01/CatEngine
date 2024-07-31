#include "cepch.h"
#include "WindowsWindow.h"

// Graphics Contexts
#include "Platform/OpenGL/OpenGLGraphicsContext.h"


// Event
#include "CatEngine/Events/ApplicationEvent.h"
#include "CatEngine/Events/KeyEvent.h"
#include "CatEngine/Events/MouseEvent.h"



namespace CatEngine {
	// Setting GLFW Functions and Variables
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		API_ERROR("GLFW Error ({0}) : {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		CE_PROFILE_FUNCTION();
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
		CE_PROFILE_FUNCTION();
		Shutdown();
	}
	void WindowsWindow::OnUpdate()
	{
		CE_PROFILE_FUNCTION();
		glfwPollEvents();
		m_Context->SwapBuffers();
	}
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}
	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_Window;
	}
	
	Scope<Window> Window::Create(const WindowProps& props) {
		return CreateScope<WindowsWindow>(props);
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		CE_PROFILE_FUNCTION();
		// Setting Data props to equal inputted props
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		API_INFO("Creating Window {0}({1}, {2})", props.Title, props.Width, props.Height);


		// Initializing GLFW
		if (!s_GLFWInitialized) {
			int success = glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);
			API_ASSERT(success, "Could not load GLFW!");
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLGraphicsContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Setting GLFW Callbacks-------------------------------------------------------------------

		// Close Callback
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		// Resize Callback
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;


			WindowResizeEvent event(width, height);
			

			data.EventCallback(event);
		});
		
		// Focus Callback
		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			if(focused)
			{
				WindowFocusEvent event;
				data.EventCallback(event);
			}
			else {
				WindowLostFocusEvent event;
				data.EventCallback(event);
			}
		});


		// Key Callback
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			// TODO : Convert to CEKeys to generalize for crossplatform
			switch (action)
			{
				case GLFW_PRESS: 
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		// Key Typed Callback
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(codepoint);
			data.EventCallback(event);
		});

		// Mouse Callback
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			// TODO : Convert to CEKeys to generalize for crossplatform
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		// Mouse Scroll Callback
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.EventCallback(event);
		});

		// Mouse Position Callback
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((unsigned int)xpos, (unsigned int)ypos);
			data.EventCallback(event);
		});
	}
	void WindowsWindow::Shutdown()
	{
		CE_PROFILE_FUNCTION();
		glfwDestroyWindow(m_Window);
	}
}

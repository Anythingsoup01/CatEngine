#include "cepch.h"
#include "WindowsInput.h"
#include "CatEngine/Application.h"

#include "GLFW/glfw3.h"

namespace CatEngine {

	Input* Input::s_Instance = new WindowsInput;

	bool WindowsInput::KeyImpl(int keycode, int keyState)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == keyState;
	}

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		return KeyImpl(keycode, GLFW_PRESS);
	}

	bool WindowsInput::IsKeyReleasedImpl(int keycode)
	{
		return KeyImpl(keycode, GLFW_RELEASE);
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePositionImpl();

		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();

		return y;
	}
}
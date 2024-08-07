#pragma once
#include "CatEngine/Core/Core.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace CatEngine {

	class Input {
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsKeyReleased(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
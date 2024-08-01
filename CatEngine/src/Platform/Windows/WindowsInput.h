#pragma once
#include "CatEngine/Core/Input/Input.h"

namespace CatEngine {
	class WindowsInput : public Input {
	protected:
		virtual bool KeyImpl(int keycode, int keyState) override;
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsKeyReleasedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}
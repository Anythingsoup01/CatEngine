#pragma once

#include "EventHandler.h"

namespace CatEngine {

	class MouseMovedEvent : public EventHandler {
	private:
		unsigned int m_MouseX, m_MouseY;
	public:
		MouseMovedEvent(unsigned int mouseX, unsigned int mouseY)
			: m_MouseX(mouseX), m_MouseY(mouseY) {}

		inline unsigned int GetMouseX() const { return m_MouseX; }
		inline unsigned int GetMouseY() const { return m_MouseY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "Mouse Moved Event: x" << m_MouseX << ", y" << m_MouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(MouseEvent | InputEvent)
	};

	class MouseScrolledEvent : public EventHandler {
	private:
		unsigned int m_OffsetX, m_OffsetY;
	public:
		MouseScrolledEvent(unsigned int offsetX, unsigned int offsetY)
			: m_OffsetX(offsetX), m_OffsetY(offsetY) {}

		inline unsigned int GetOffsetX() const { return m_OffsetX; }
		inline unsigned int GetOffsetY() const { return m_OffsetY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "Mouse Scrolled Event: x" << m_OffsetX << ", y" << m_OffsetY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(MouseEvent | InputEvent)
	};

	class MouseButtonEvent : public EventHandler {
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(MouseEvent | InputEvent)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}
		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "Mouse Pressed Event: " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "Mouse Released Event: " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}
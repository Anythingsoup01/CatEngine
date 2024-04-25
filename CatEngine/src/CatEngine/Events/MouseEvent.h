#pragma once

#include "Events.h"
//MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
namespace CatEngine {
	// Base Mouse Button Event
	class MouseOnButtonEvent : public Events {
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(MouseButtonEvent | InputEvent)
	protected:
		MouseOnButtonEvent(int button)
			: m_Button(button) {}
		int m_Button;
	};
	// Mouse Button Pressed Event, using MouseOnButtonEvent
	class MouseButtonPressedEvent : public MouseOnButtonEvent {
	public:
		MouseButtonPressedEvent(int button)
			: MouseOnButtonEvent(button) {}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "Mouse Button Pressed Event " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};
	// Mouse Button Pressed Event, using MouseOnButtonEvent
	class MouseButtonReleasedEvent : public MouseOnButtonEvent {
	public:
		MouseButtonReleasedEvent(int button)
			: MouseOnButtonEvent(button) {}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "Mouse Button Released Event " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	// Mouse Moved Event
	class MouseMovedEvent : public Events {
	public:
		MouseMovedEvent(unsigned int mouseX, unsigned int mouseY)
			: m_MouseX(mouseX), m_MouseY(mouseY) {}

		inline unsigned int GetMouseX() const { return m_MouseX; }
		inline unsigned int GetMouseY() const { return m_MouseY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "Mouse Moved Event: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(MouseEvent | InputEvent)
	private:
		unsigned int m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Events {
	public:
		MouseScrolledEvent(unsigned int offsetX, unsigned int offsetY)
			: m_OffsetX(offsetX), m_OffsetY(offsetY) {}

		inline unsigned int GetMouseX() const { return m_OffsetX; }
		inline unsigned int GetMouseY() const { return m_OffsetY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "Mouse Scrolled Event: " << m_OffsetX << ", " << m_OffsetY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(MouseEvent | InputEvent)
	private:
		unsigned int m_OffsetX, m_OffsetY;
	};
}
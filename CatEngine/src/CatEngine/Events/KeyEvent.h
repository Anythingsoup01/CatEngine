#pragma once

#include "Events.h"

namespace CatEngine {
	// Base Key Event Class
	class KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(CategoryInputEvent | CategoryKeyboardEvent)
	protected:
		KeyEvent(int keycode)
		: m_KeyCode(keycode) {}
		int m_KeyCode;
	};

	// Key Pressed Event, using KeyEvent
	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}
		inline int GetRepeatCount() const { return m_RepeatCount; }
		std::string ToString() const override {
			std::stringstream ss;
			ss << "Key Press Event " << m_KeyCode << " (" << m_RepeatCount << ")";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	// Key Typed Event
	
	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "Key Typed Event " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)
	};

	// Key Released Event, using Key Event
	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "Key Release Event " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};
}
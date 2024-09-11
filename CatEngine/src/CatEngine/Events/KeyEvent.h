#pragma once

#include "Events.h"
#include "CatEngine/Core/KeyCodes.h"

namespace CatEngine {
	// Base Key Event Class
	class KeyEvent : public Event {
	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(CategoryInputEvent | CategoryKeyboardEvent)
	protected:
		KeyEvent(const KeyCode keycode)
		: m_KeyCode(keycode) {}
		KeyCode m_KeyCode;
	};

	// Key Pressed Event, using KeyEvent
	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(const KeyCode keycode, int repeatCount)
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
		KeyTypedEvent(const KeyCode keycode)
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
		KeyReleasedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "Key Release Event " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};
}

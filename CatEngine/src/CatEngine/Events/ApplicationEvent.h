#pragma once

#include "Events.h"

namespace CatEngine {
	// Window Close Event
	class WindowCloseEvent : public Events {
	public:
		WindowCloseEvent() {}
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(CategoryApplicationEvent)
	};
	// Window Resize Event
	class WindowResizeEvent : public Events {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "Window Resize Event: " << m_Width << ", " << m_Height;
			return ss.str();
		}
		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(CategoryApplicationEvent)
	private:
		unsigned int m_Width, m_Height;
	};
	// Window Focus Event
	class WindowFocusEvent : public Events {
	public:
		WindowFocusEvent() {}
		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(CategoryApplicationEvent)
	};
	// Window Lost Focus Event
	class WindowLostFocusEvent : public Events {
	public:
		WindowLostFocusEvent() {}
		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CATEGORY(CategoryApplicationEvent)
	};
	// Window Moved Event
	class WindowMovedEvent : public Events {
	public:
		WindowMovedEvent() {}
		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(CategoryApplicationEvent)
	};
	// Window Maximized
	class WindowMaximizedEvent : public Events {
	public:
		WindowMaximizedEvent() {}
		EVENT_CLASS_TYPE(WindowMaximized)
		EVENT_CLASS_CATEGORY(CategoryApplicationEvent)
	};
	// Window Minimized Event
	class WindowMinimizedEvent : public Events {
	public:
		WindowMinimizedEvent() {}
		EVENT_CLASS_TYPE(WindowMinimized)
		EVENT_CLASS_CATEGORY(CategoryApplicationEvent)
	};
}

#pragma once
#include "cepch.h"
#include "CatEngine/Core/Core.h"

namespace CatEngine {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, WindowMaximized,
		KeyPressed, KeyTyped, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};

	enum EventCategory { 
		None = 0, 
		CategoryApplicationEvent   = BIT(0),	
		CategoryInputEvent         = BIT(1),
		CategoryKeyboardEvent      = BIT(2),
		CategoryMouseEvent         = BIT(3),
		CategoryMouseButtonEvent   = BIT(4),
	};

/* The event type lets the Event Dispatcher determine what the event is and runs any premade scripts or custom scripts.
Defaults to None, but can take WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, KeyPressed, KeyReleased, MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled events*/
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
                               virtual EventType GetEventType() const override { return GetStaticType(); }\
                               virtual const char* GetName() const override { return #type; }
/* The event category is used to group the event into what they apply to.
This can range from CategoryApplicationEvent, CategoryInputEvent, CategoryKeyboardEvent, CategoryMouseEvent, CategoryMouseButtonEvent*/
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event {
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
		: m_Event(event) {}

		template<typename T, typename F>
		bool Dispatch(const F& func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
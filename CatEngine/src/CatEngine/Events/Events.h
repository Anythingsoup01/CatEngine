#pragma once
#include "cepch.h"
#include "CatEngine/Core/Core.h"

namespace CatEngine {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, WindowMaximized, WindowMinimized,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};

	enum EventCategory { 
		None = 0, 
		ApplicationEvent   = BIT(0),	
		InputEvent         = BIT(1),
		KeyboardEvent      = BIT(2),
		MouseEvent         = BIT(3),
		MouseButtonEvent   = BIT(4),
	};

/* The event type lets the Event Dispatcher determine what the event is and runs any premade scripts or custom scripts.
Defaults to None, but can take WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, WindowMinimized, KeyPressed, KeyReleased, MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled events*/
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
                               virtual EventType GetEventType() const override { return GetStaticType(); }\
                               virtual const char* GetName() const override { return #type; }
/* The event category is used to group the event into what they apply to.
This can range from ApplicationEvent, InputEvent, KeyboardEvent, MouseEvent, MouseButtonEvent*/
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Events {
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;
	};

	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Events& event)
		: m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Events& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Events& e)
	{
		return os << e.ToString();
	}
}
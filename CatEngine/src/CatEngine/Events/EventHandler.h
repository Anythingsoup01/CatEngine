#pragma once
#include "cepch.h"
#include "CatEngine/Core/Core.h"


namespace CatEngine {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};

	enum EventCategory {
		None = 0,
		ApplicationEvent = BIT(0),
		InputEvent = BIT(1),
		KeyboardEvent = BIT(2),
		MouseEvent = BIT(3),
		MouseButtonEvent = BIT(4)
	};

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type; }\
									virtual EventType GetEventType() const override { return GetStaticType(); }\
									virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int  GetCategoryFlags() const override { return category; }

	class EventHandler {
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
	private:
		EventHandler& m_Event;
	public:
		EventDispatcher(EventHandler& event)
			: m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) 
		{
			if (m_Event.GetEventType() == T::GetStaticType()) 
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	};

	inline std::ostream& operator << (std::ostream& os, const EventHandler& e)
	{
		return os << e.ToString();
	}
}

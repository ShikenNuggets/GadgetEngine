#ifndef GADGET_EVENT_H
#define GADGET_EVENT_H

#include <functional>

#include "Utils/StringID.h"

namespace Gadget{
	enum class EventType{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MouseMoved, MouseScroll,
		MouseButtonPressed, MouseButtonReleased
	};

	class Event{
	public:
		Event(StringID name_) : name(name_), handled(false){}

		virtual EventType GetEventType() const = 0;
		inline StringID GetName() const{ return name; }
		
		virtual std::string ToString() const{ return StringID::GetStringFromID(GetName()); }

	protected:
		StringID name;
		bool handled;

	private:
		friend class EventDispatcher;
	};

	//TODO - Not sure how I feel about this whole thing
	class EventDispatcher{
		template <class T>
		using EventFn = std::function<bool>(T&);

	public:
		EventDispatcher(Event& event_) : m_event(event_){}

		template<class T>
		bool Dispatch(EventFn<T> func_){
			static_assert(m_event.GetEventType() == T::GetStaticType());
			if(m_event.GetEventType() == T::Type()){
				m_event.handled = func(*(T*)&m_event);
				return true;
			}

			return false;
		}

	private:
		Event& m_event;
	};
}

#endif //!GADGET_EVENT_H
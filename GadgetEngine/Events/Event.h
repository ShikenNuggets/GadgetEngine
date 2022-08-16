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
		MouseButtonPressed, MouseButtonReleased,
		GamepadAxis, GamepadButtonPressed, GamepadButtonReleased,

		Count //DO NOT PUT ANYTHING AFTER THIS
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
	};
}

#endif //!GADGET_EVENT_H
#ifndef GADGET_EVENT_H
#define GADGET_EVENT_H

#include "Utils/StringID.h"

namespace Gadget{
	enum class EventType : uint8_t{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, WindowRestarted,
		KeyPressed, KeyReleased,
		MouseMoved, MouseScroll,
		MouseButtonPressed, MouseButtonReleased,
		GamepadAxis, GamepadButtonPressed, GamepadButtonReleased,

		Count //DO NOT PUT ANYTHING AFTER THIS
	};

	class Event{
	public:
		Event(StringID name_) : name(name_), handled(false){}
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		inline StringID GetName() const{ return name; }
		
		virtual std::string ToString() const{ return name.GetString(); }

	protected:
		StringID name;
		bool handled;
	};
}

#endif //!GADGET_EVENT_H
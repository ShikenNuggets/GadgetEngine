#ifndef GADGET_KEY_EVENT_H
#define GADGET_KEY_EVENT_H

#include "InputEnums.h"
#include "Events/Event.h"

namespace Gadget{
	class KeyEvent : public Event{
	public:
		inline ButtonID GetKeyCode() const{ return keycode; }

	protected:
		KeyEvent(ButtonID keycode_, StringID name_) : Event(name_), keycode(keycode_){}
		virtual ~KeyEvent(){}

		ButtonID keycode;
	};

	class KeyPressedEvent : public KeyEvent{
	public:
		KeyPressedEvent(ButtonID keycode_) : KeyEvent(keycode_, SID("KeyPressedEvent")), repeatCount(0){}

		static constexpr EventType Type(){ return EventType::KeyPressed; }
		virtual EventType GetEventType() const final override{ return Type(); }

		virtual std::string ToString() const final override{ return std::string(StringID::GetStringFromID(name)) + ": " + std::to_string((int)keycode) + " (" + std::to_string(repeatCount) + " repeats)"; }

	private:
		int repeatCount;
	};

	class KeyReleasedEvent : public KeyEvent{
	public:
		KeyReleasedEvent(ButtonID keycode_) : KeyEvent(keycode_, SID("KeyReleasedEvent")){}

		static constexpr EventType Type(){ return EventType::KeyReleased; }
		virtual EventType GetEventType() const final override{ return Type(); }

		virtual std::string ToString() const final override{ return std::string(StringID::GetStringFromID(name)) + ": " + std::to_string((int)keycode); }
	};
}

#endif //!GADGET_KEY_EVENT_H
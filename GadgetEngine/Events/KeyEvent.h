#ifndef GADGET_KEY_EVENT_H
#define GADGET_KEY_EVENT_H

#include "Event.h"

namespace Gadget{
	class KeyEvent : public Event{
	public:
		inline int GetKeyCode() const{ return keycode; }

	protected:
		KeyEvent(int keycode_, StringID name_) : Event(name_), keycode(keycode_){}
		int keycode;
	};

	class KeyPressedEvent : public KeyEvent{
	public:
		KeyPressedEvent(int keycode_) : KeyEvent(keycode_, StringID::InternString("KeyPressedEvent")), repeatCount(0){}

		static constexpr EventType Type(){ return EventType::KeyPressed; }
		virtual EventType GetEventType() const override{ return Type(); }

		virtual std::string ToString() const override{ return StringID::GetStringFromID(name) + ": " + std::to_string(keycode) + " (" + std::to_string(repeatCount) + " repeats)"; }

	private:
		int repeatCount;
	};

	class KeyReleasedEvent : public KeyEvent{
	public:
		KeyReleasedEvent(int keycode_) : KeyEvent(keycode_, StringID::InternString("KeyReleasedEvent")){}

		static constexpr EventType Type(){ return EventType::KeyReleased; }
		virtual EventType GetEventType() const override{ return Type(); }

		virtual std::string ToString() const override{ return StringID::GetStringFromID(name) + ": " + std::to_string(keycode); }
	};
}

#endif //!GADGET_KEY_EVENT_H
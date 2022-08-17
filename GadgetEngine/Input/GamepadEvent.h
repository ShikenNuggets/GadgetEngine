#ifndef GAMEPAD_EVENT_H
#define GAMEPAD_EVENT_H

#include "InputEnums.h"
#include "Events/Event.h"

namespace Gadget{
	class GamepadEvent : public Event{
	public:
	protected:
		GamepadEvent(StringID name_, int joystickIndex_) : Event(name_), joystickIndex(joystickIndex_){}

		int joystickIndex;
	};

	class GamepadAxisEvent : public GamepadEvent{
	public:
		GamepadAxisEvent(int joystickIndex_, AxisID axisID_, float value_) : GamepadEvent(SID("GamepadAxisEvent"), joystickIndex_), axisID(axisID_), value(value_){}

		static constexpr EventType Type(){ return EventType::GamepadAxis; }
		virtual EventType GetEventType() const override{ return Type(); }

		AxisID GetAxisIndex() const{ return axisID; }
		float GetValue() const{ return value; }

		virtual std::string ToString() const override{ return name.GetString() + ": " + std::to_string((int)axisID); }

	protected:
		AxisID axisID;
		float value;
	};

	class GamepadButtonEvent : public GamepadEvent{
	public:
		inline ButtonID GetButton() const{ return button; }

	protected:
		GamepadButtonEvent(StringID name_, int joystickIndex_, ButtonID button_) : GamepadEvent(name_, joystickIndex_), button(button_){}

		ButtonID button;
	};

	class GamepadButtonPressedEvent : public GamepadButtonEvent{
	public:
		GamepadButtonPressedEvent(int joystickIndex_, ButtonID button_) : GamepadButtonEvent(SID("GamepadButtonPressedEvent"), joystickIndex_, button_){}

		static constexpr EventType Type(){ return EventType::GamepadButtonPressed; }
		virtual EventType GetEventType() const override{ return Type(); }

		virtual std::string ToString() const override{ return name.GetString() + ": " + std::to_string((int)button); }
	};

	class GamepadButtonReleasedEvent : public GamepadButtonEvent{
	public:
		GamepadButtonReleasedEvent(int joystickIndex_, ButtonID button_) : GamepadButtonEvent(SID("MouseButtonReleasedEvent"), joystickIndex_, button_){}

		static constexpr EventType Type(){ return EventType::GamepadButtonReleased; }
		virtual EventType GetEventType() const override{ return Type(); }

		virtual std::string ToString() const override{ return std::string(StringID::GetStringFromID(name)) + ": " + std::to_string((int)button); }
	};
}

#endif // !GAMEPAD_EVENT_H
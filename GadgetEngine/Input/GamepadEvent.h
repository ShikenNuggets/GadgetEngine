#ifndef GAMEPAD_EVENT_H
#define GAMEPAD_EVENT_H

#include "Input.h"
#include "InputEnums.h"
#include "Events/Event.h"

namespace Gadget{
	class GamepadEvent : public Event{
	public:
	protected:
		GamepadEvent(StringID name_, int joystickIndex_) : Event(name_), joystickIndex(joystickIndex_){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(joystickIndex_ < Input::MaxGamepads);
		}

		virtual ~GamepadEvent(){}

		int joystickIndex;
	};

	class GamepadAxisEvent : public GamepadEvent{
	public:
		GamepadAxisEvent(int joystickIndex_, AxisID axisID_, float value_) : GamepadEvent(SID("GamepadAxisEvent"), joystickIndex_), axisID(axisID_), value(value_){
			GADGET_BASIC_ASSERT(joystickIndex_ < Input::MaxGamepads);
			GADGET_BASIC_ASSERT(axisID_ < AxisID::AxisID_MAX);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(value_));
		}

		virtual ~GamepadAxisEvent(){}

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
		GamepadButtonEvent(StringID name_, int joystickIndex_, ButtonID button_) : GamepadEvent(name_, joystickIndex_), button(button_){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(joystickIndex_ < Input::MaxGamepads);
			GADGET_BASIC_ASSERT(button_ < ButtonID::ButtonID_MAX);
		}

		virtual ~GamepadButtonEvent(){}

		ButtonID button;
	};

	class GamepadButtonPressedEvent : public GamepadButtonEvent{
	public:
		GamepadButtonPressedEvent(int joystickIndex_, ButtonID button_) : GamepadButtonEvent(SID("GamepadButtonPressedEvent"), joystickIndex_, button_){
			GADGET_BASIC_ASSERT(joystickIndex_ < Input::MaxGamepads);
			GADGET_BASIC_ASSERT(button_ < ButtonID::ButtonID_MAX);
		}

		static constexpr EventType Type(){ return EventType::GamepadButtonPressed; }
		virtual EventType GetEventType() const final override{ return Type(); }

		virtual std::string ToString() const final override{ return name.GetString() + ": " + std::to_string((int)button); }
	};

	class GamepadButtonReleasedEvent : public GamepadButtonEvent{
	public:
		GamepadButtonReleasedEvent(int joystickIndex_, ButtonID button_) : GamepadButtonEvent(SID("MouseButtonReleasedEvent"), joystickIndex_, button_){
			GADGET_BASIC_ASSERT(joystickIndex_ < Input::MaxGamepads);
			GADGET_BASIC_ASSERT(button_ < ButtonID::ButtonID_MAX);
		}

		static constexpr EventType Type(){ return EventType::GamepadButtonReleased; }
		virtual EventType GetEventType() const final override{ return Type(); }

		virtual std::string ToString() const final override{ return std::string(StringID::GetStringFromID(name)) + ": " + std::to_string((int)button); }
	};
}

#endif //!GAMEPAD_EVENT_H
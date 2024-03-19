#ifndef GADGET_MOUSE_EVENT_H
#define GADGET_MOUSE_EVENT_H

#include "InputEnums.h"
#include "Events/Event.h"
#include "Math/Math.h"

namespace Gadget{
	class MouseMovedEvent : public Event{
	public:
		MouseMovedEvent(float x_, float y_, float xAbs_, float yAbs_) : Event(SID("MouseMovedEvent")), x(x_), y(y_), xAbsolute(xAbs_), yAbsolute(yAbs_){
			GADGET_BASIC_ASSERT(Math::IsValidNumber(x_));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(y_));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(xAbs_));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(yAbs_));
		}

		static constexpr EventType Type(){ return EventType::MouseMoved; }
		virtual EventType GetEventType() const final override{ return Type(); }

		inline float GetX() const{ return x; }
		inline float GetY() const{ return y; }
		inline float GetXAbsolute() const{ return xAbsolute; }
		inline float GetYAbsolute() const{ return yAbsolute; }

		virtual std::string ToString() const final override{ return std::string(StringID::GetStringFromID(name)) + ": " + std::to_string(x) + ", " + std::to_string(y); }

	private:
		float x;
		float y;
		float xAbsolute;
		float yAbsolute;
	};

	class MouseScrollEvent : public Event{
	public:
		MouseScrollEvent(float xOffset_, float yOffset_) : Event(SID("MouseScrollEvent")), xOffset(xOffset_), yOffset(yOffset_){
			GADGET_BASIC_ASSERT(Math::IsValidNumber(xOffset_));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(yOffset_));
		}

		static constexpr EventType Type(){ return EventType::MouseScroll; }
		virtual EventType GetEventType() const final override{ return Type(); }

		inline float GetXOffset() const{ return xOffset; }
		inline float GetYOffset() const{ return yOffset; }

		virtual std::string ToString() const final override{ return std::string(StringID::GetStringFromID(name)) + ": " + std::to_string(xOffset) + ", " + std::to_string(yOffset); }

	private:
		float xOffset;
		float yOffset;
	};

	class MouseButtonEvent : public Event{
	public:
		inline ButtonID GetButton() const{ return button; }

	protected:
		MouseButtonEvent(ButtonID button_, StringID name_) : Event(name_), button(button_){
			GADGET_BASIC_ASSERT(button_ < ButtonID::ButtonID_MAX);
			GADGET_BASIC_ASSERT(name_ != StringID::None);
		}

		virtual ~MouseButtonEvent(){}

		ButtonID button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent{
	public:
		MouseButtonPressedEvent(ButtonID button_) : MouseButtonEvent(button_, SID("MouseButtonPressedEvent")){
			GADGET_BASIC_ASSERT(button_ < ButtonID::ButtonID_MAX);
		}

		static constexpr EventType Type(){ return EventType::MouseButtonPressed; }
		virtual EventType GetEventType() const final override{ return Type(); }

		virtual std::string ToString() const final override{ return std::string(StringID::GetStringFromID(name)) + ": " + std::to_string((int)button); }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent{
	public:
		MouseButtonReleasedEvent(ButtonID button_) : MouseButtonEvent(button_, SID("MouseButtonReleasedEvent")){
			GADGET_BASIC_ASSERT(button_ < ButtonID::ButtonID_MAX);
		}

		static constexpr EventType Type(){ return EventType::MouseButtonReleased; }
		virtual EventType GetEventType() const final override{ return Type(); }

		virtual std::string ToString() const final override{ return std::string(StringID::GetStringFromID(name)) + ": " + std::to_string((int)button); }
	};
}

#endif //!GADGET_MOUSE_EVENT_H
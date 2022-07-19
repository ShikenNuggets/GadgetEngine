#ifndef GADGET_MOUSE_EVENT_H
#define GADGET_MOUSE_EVENT_H

#include "Event.h"

namespace Gadget{
	class MouseMovedEvent : public Event{
	public:
		MouseMovedEvent(int x_, int y_) : Event(StringID::InternString("MouseMovedEvent")), x(x_), y(y_){}

		static constexpr EventType Type(){ return EventType::MouseMoved; }
		virtual EventType GetEventType() const override{ return Type(); }

		inline int GetX() const{ return x; }
		inline int GetY() const{ return y; }

		virtual std::string ToString() const override{ return StringID::GetStringFromID(name) + ": " + std::to_string(x) + ", " + std::to_string(y); }

	private:
		int x;
		int y;
	};

	class MouseScrollEvent : public Event{
	public:
		MouseScrollEvent(int xOffset_, int yOffset_) : Event(StringID::InternString("MouseScrollEvent")), xOffset(xOffset_), yOffset(yOffset_){}

		static constexpr EventType Type(){ return EventType::MouseScroll; }
		virtual EventType GetEventType() const override{ return Type(); }

		inline int GetXOffset() const{ return xOffset; }
		inline int GetYOffset() const{ return yOffset; }

		virtual std::string ToString() const override{ return StringID::GetStringFromID(name) + ": " + std::to_string(xOffset) + ", " + std::to_string(yOffset); }

	private:
		int xOffset;
		int yOffset;
	};

	class MouseButtonEvent : public Event{
	public:
		inline int GetButton() const{ return button; }

	protected:
		MouseButtonEvent(int button_, StringID name_) : Event(name_), button(button_){}

		int button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent{
	public:
		MouseButtonPressedEvent(int button_) : MouseButtonEvent(button_, StringID::InternString("MouseButtonPressedEvent")){}

		static constexpr EventType Type(){ return EventType::MouseButtonPressed; }
		virtual EventType GetEventType() const override{ return Type(); }

		virtual std::string ToString() const override{ return StringID::GetStringFromID(name) + ": " + std::to_string(button); }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent{
	public:
		MouseButtonReleasedEvent(int button_) : MouseButtonEvent(button_, StringID::InternString("MouseButtonReleasedEvent")){}

		static constexpr EventType Type(){ return EventType::MouseButtonReleased; }
		virtual EventType GetEventType() const override{ return Type(); }

		virtual std::string ToString() const override{ return StringID::GetStringFromID(name) + ": " + std::to_string(button); }
	};
}

#endif //!GADGET_MOUSE_EVENT_H
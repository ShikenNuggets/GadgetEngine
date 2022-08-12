#ifndef GADGET_APP_EVENT_H
#define GADGET_APP_EVENT_H

#include "Event.h"

namespace Gadget{
	class WindowResizedEvent : public Event{
	public:
		WindowResizedEvent(int width_, int height_) : Event(StringID::InternString("WindowResizedEvent")), width(width_), height(height_){}

		static constexpr EventType Type(){ return EventType::WindowResize; }
		virtual EventType GetEventType() const override{ return Type(); }

		inline int GetWidth() const{ return width; }
		inline int GetHeight() const{ return height; }

		virtual std::string ToString() const override{ return std::string(StringID::GetStringFromID(name)) + ": " + std::to_string(width) + ", " + std::to_string(height); }

	private:
		int width;
		int height;
	};

	class WindowCloseEvent : public Event{
	public:
		WindowCloseEvent() : Event(StringID::InternString("WindowCloseEvent")){}

		static constexpr EventType Type(){ return EventType::WindowClose; }
		virtual EventType GetEventType() const override{ return Type(); }
	};
}

#endif //!GADGET_APP_EVENT_H
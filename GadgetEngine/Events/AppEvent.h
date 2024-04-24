#ifndef GADGET_APP_EVENT_H
#define GADGET_APP_EVENT_H

#include "Events/Event.h"

namespace Gadget{
	class WindowResizedEvent : public Event{
	public:
		WindowResizedEvent(int width_, int height_) : Event(SID("WindowResizedEvent")), width(width_), height(height_){}

		static constexpr EventType Type(){ return EventType::WindowResize; }
		virtual EventType GetEventType() const final override{ return Type(); }

		inline int GetWidth() const{ return width; }
		inline int GetHeight() const{ return height; }

		virtual std::string ToString() const final override{ return std::string(StringID::GetStringFromID(name)) + ": " + std::to_string(width) + ", " + std::to_string(height); }

	private:
		int width;
		int height;
	};

	class WindowCloseEvent : public Event{
	public:
		WindowCloseEvent() : Event(SID("WindowCloseEvent")){}

		static constexpr EventType Type(){ return EventType::WindowClose; }
		virtual EventType GetEventType() const final override{ return Type(); }
	};

	class WindowMovedEvent : public Event{
	public:
		WindowMovedEvent(int x_, int y_) : Event(SID("WindowMovedEvent")), x(x_), y(y_){}

		static constexpr EventType Type(){ return EventType::WindowMoved; }
		virtual EventType GetEventType() const final override { return Type(); }

		inline int GetX() const{ return x; }
		inline int GetY() const{ return y; }

	private:
		int x;
		int y;
	};
}

#endif //!GADGET_APP_EVENT_H
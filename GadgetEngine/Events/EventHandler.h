#ifndef GADGET_EVENT_HANDLER_H
#define GADGET_EVENT_HANDLER_H

#include <functional>
#include <map>
#include <vector>

#include "Event.h"

namespace Gadget{
	class EventHandler{
	public:
		static EventHandler* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		void SetEventCallback(EventType type_, std::function<void(const Event&)> callback_);

		void HandleEvent(const Event& e_);

	private:
		static EventHandler* instance;

		EventHandler();
		~EventHandler();

		std::map<EventType, std::vector<std::function<void(const Event&)>>> eventCallbacks;
	};
}

#endif //!GADGET_EVENT_HANDLER_H
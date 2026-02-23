#ifndef GADGET_EVENT_HANDLER_H
#define GADGET_EVENT_HANDLER_H

#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <unordered_map>

#include "Events/Event.h"

namespace Gadget{
	using FuncPointerHashTable = std::unordered_map<void*, std::vector<std::function<void(const Event&)>>>;

	class EventHandler{
	public:
		EventHandler();
		~EventHandler();

		static EventHandler* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		void RegisterCallback(EventType type_, void* obj_, const std::function<void(const Event&)>& callback_);
		void UnregisterCallback(EventType type_, void* obj_); //Unregister a specific object for a specific event type
		void UnregisterCallbacks(void* obj_); //Unregister a specific object for *all* event types

		void HandleEvent(const Event& e_);

	private:
		static std::unique_ptr<EventHandler> instance;

		std::unordered_map<EventType, FuncPointerHashTable> eventCallbacks;
	};
}

#endif //!GADGET_EVENT_HANDLER_H
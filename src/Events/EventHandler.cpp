#include "EventHandler.h"

#include "Debug.h"

using namespace Gadget;

std::unique_ptr<EventHandler> EventHandler::instance = nullptr;

EventHandler::EventHandler() : eventCallbacks(static_cast<size_t>(EventType::Count)){
	for(EventType i = EventType::None; i < EventType::Count; i = static_cast<EventType>(static_cast<size_t>(i) + 1)){
		eventCallbacks.Add(i, FuncPointerHashTable(4096));
	}
}

EventHandler::~EventHandler(){
	eventCallbacks.Clear();
}

EventHandler* EventHandler::GetInstance(){
	if(instance == nullptr){
		instance = std::make_unique<EventHandler>();
	}

	return instance.get();
}

#ifdef GADGET_DEBUG
void EventHandler::DeleteInstance(){
	if(instance != nullptr){
		instance.reset();
	}
}
#endif //GADGET_DEBUG

void EventHandler::RegisterCallback(EventType type_, void* obj_, const std::function<void(const Event&)>& callback_){
	GADGET_BASIC_ASSERT(type_ > EventType::None && type_ < EventType::Count);
	//obj pointer is just used for bookkeeping, so it's allowed to be nullptr
	eventCallbacks[type_][obj_].Add(callback_);
}

void EventHandler::UnregisterCallback(EventType type_, void* obj_){
	GADGET_BASIC_ASSERT(type_ > EventType::None && type_ < EventType::Count);
	eventCallbacks[type_][obj_].Clear();
}

void EventHandler::UnregisterCallbacks(void* obj_){
	for(auto& [key, value] : eventCallbacks){
		value[obj_].Clear();
	}
}

void EventHandler::HandleEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() != EventType::None);
	GADGET_BASIC_ASSERT(e_.GetEventType() < EventType::Count);

	for(auto& [key, value] : eventCallbacks[e_.GetEventType()]){
		for(const auto& callback : value){
			callback(e_);
		}
	}
}
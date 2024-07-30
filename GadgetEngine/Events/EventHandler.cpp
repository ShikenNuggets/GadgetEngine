#include "EventHandler.h"

#include "Debug.h"

using namespace Gadget;

std::unique_ptr<EventHandler> EventHandler::instance = nullptr;

EventHandler::EventHandler() : eventCallbacks(){
	for(EventType i = EventType::None; i < EventType::Count; i = static_cast<EventType>(static_cast<size_t>(i) + 1)){
		eventCallbacks.emplace(i, std::vector<std::function<void(const Event&)>>());
	}
}

EventHandler::~EventHandler(){
	eventCallbacks.clear();
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

void EventHandler::SetEventCallback(EventType type_, const std::function<void(const Event&)>& callback_){
	GADGET_BASIC_ASSERT(type_ != EventType::None);
	GADGET_BASIC_ASSERT(type_ < EventType::Count);
	GADGET_ASSERT(eventCallbacks.find(type_) != eventCallbacks.end(), "Tried to set event callback of invalid type!");

	eventCallbacks[type_].push_back(callback_);
}

void EventHandler::HandleEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() != EventType::None);
	GADGET_BASIC_ASSERT(e_.GetEventType() < EventType::Count);
	GADGET_BASIC_ASSERT(eventCallbacks.size() == (int)EventType::Count);

	for(auto& callback : eventCallbacks[e_.GetEventType()]){
		callback(e_);
	}
}
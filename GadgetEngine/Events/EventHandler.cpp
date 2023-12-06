#include "EventHandler.h"

#include "Debug.h"

using namespace Gadget;

EventHandler* EventHandler::instance = nullptr;

EventHandler::EventHandler() : eventCallbacks(){
	for(size_t i = 0; i < (size_t)EventType::Count; i++){
		eventCallbacks.emplace((EventType)i, std::vector<std::function<void(const Event&)>>());
	}
}

EventHandler::~EventHandler(){
	eventCallbacks.clear();
}

EventHandler* EventHandler::GetInstance(){
	if(instance == nullptr){
		instance = new EventHandler();
	}

	return instance;
}

#ifdef GADGET_DEBUG
void EventHandler::DeleteInstance(){
	if(instance != nullptr){
		delete instance;
		instance = nullptr;
	}
}
#endif //GADGET_DEBUG

void EventHandler::SetEventCallback(EventType type_, std::function<void(const Event&)> callback_){
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
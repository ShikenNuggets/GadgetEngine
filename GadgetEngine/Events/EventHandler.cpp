#include "EventHandler.h"

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
	_ASSERT(eventCallbacks.find(type_) != eventCallbacks.end());

	eventCallbacks[type_].push_back(callback_);
}

void EventHandler::HandleEvent(const Event& e_){
	for(auto& callback : eventCallbacks[e_.GetEventType()]){
		callback(e_);
	}
}
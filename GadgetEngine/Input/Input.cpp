#include "Input.h"

#include "Events/EventHandler.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

using namespace Gadget;

Input* Input::instance = nullptr;

Input::Input() : buttons(), axes(){
	EventHandler::GetInstance()->SetEventCallback(EventType::KeyPressed, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::KeyReleased, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseMoved, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseScroll, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseButtonPressed, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseButtonReleased, OnEvent);

	buttons.reserve(256);
	axes.reserve(256);
}

Input::~Input(){}

Input* Input::GetInstance(){
	if(instance == nullptr){
		instance = new Input();
	}

	return instance;
}

#ifdef GADGET_DEBUG
void Input::DeleteInstance(){
	if(instance != nullptr){
		delete instance;
		instance = nullptr;
	}
}
#endif //GADGET_DEBUG

void Input::ProcessInputs(){
	//TODO
}

void Input::OnEvent(const Event& e_){
	switch(e_.GetEventType()){
		case EventType::KeyPressed:
			GetInstance()->buttons.push_back(Button(dynamic_cast<const KeyPressedEvent&>(e_).GetKeyCode(), true));
			break;
		case EventType::KeyReleased:
			GetInstance()->buttons.push_back(Button(dynamic_cast<const KeyReleasedEvent&>(e_).GetKeyCode(), false));
			break;
		case EventType::MouseMoved:
			GetInstance()->axes.push_back(Axis(AxisID::Mouse_Move_Horizontal, dynamic_cast<const MouseMovedEvent&>(e_).GetX()));
			GetInstance()->axes.push_back(Axis(AxisID::Mouse_Move_Vertical, dynamic_cast<const MouseMovedEvent&>(e_).GetY()));
			break;
		case EventType::MouseScroll:
			GetInstance()->axes.push_back(Axis(AxisID::Mouse_Scroll_Horizontal, dynamic_cast<const MouseScrollEvent&>(e_).GetXOffset()));
			GetInstance()->axes.push_back(Axis(AxisID::Mouse_Scroll_Vertical, dynamic_cast<const MouseScrollEvent&>(e_).GetYOffset()));
			break;
		case EventType::MouseButtonPressed:
			GetInstance()->buttons.push_back(Button(dynamic_cast<const MouseButtonPressedEvent&>(e_).GetButton(), true));
			break;
		case EventType::MouseButtonReleased:
			GetInstance()->buttons.push_back(Button(dynamic_cast<const MouseButtonReleasedEvent&>(e_).GetButton(), false));
			break;
		default:
			Debug::Log("Unhandled Event Type [" + e_.GetName().GetString() + "] in Input!", Debug::Error, __FILE__, __LINE__);
			break;
	}
}
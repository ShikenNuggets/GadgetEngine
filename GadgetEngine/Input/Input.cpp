#include "Input.h"

#include "Events/EventHandler.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

using namespace Gadget;

Input* Input::instance = nullptr;

Input::Input() : buttonEvents(), axisEvents(), buttonsDown(), buttonsUp(), buttonsHeld(), axes(){
	EventHandler::GetInstance()->SetEventCallback(EventType::KeyPressed, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::KeyReleased, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseMoved, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseScroll, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseButtonPressed, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseButtonReleased, OnEvent);

	buttonEvents.reserve(256);
	axisEvents.reserve(256);
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

bool Input::GetButtonDown(ButtonID id_) const{
	return buttonsDown.find(id_) != buttonsDown.end();
}

bool Input::GetButtonUp(ButtonID id_) const{
	return buttonsUp.find(id_) != buttonsUp.end();
}

bool Input::GetButtonHeld(ButtonID id_) const{
	return buttonsHeld.find(id_) != buttonsHeld.end();
}

float Input::GetAxis(AxisID id_) const{
	auto it = axes.find(id_);
	if(it != axes.end()){
		return axes.at(id_);
	}

	return 0.0f;
}

//Note - The implementation here could have some weirdness when pressing the same button multiple times/shifting an axis a lot on a single frame
//I only expect this to be an issue on frames with major lag spikes, which ideally the high-level parts of the input system/game will be somewhat resilient to
void Input::ProcessInputs(){
	buttonsDown.clear();
	buttonsUp.clear();
	axes.clear();

	for(const auto& b : buttonEvents){
		if(b.IsPressed()){
			buttonsDown.insert(b.GetButtonID());
			buttonsHeld.insert(b.GetButtonID());
		}else{
			buttonsUp.insert(b.GetButtonID());
		}
	}

	for(const auto& a : axisEvents){
		if(axes.find(a.GetAxisID()) != axes.end()){
			axes[a.GetAxisID()] += a.Value(); //Update our existing axis value, in case there are multiple events for the same axis (common with mouse motion)
		}else{
			axes.insert(std::make_pair(a.GetAxisID(), a.Value()));
		}
	}
}

void Input::OnEvent(const Event& e_){
	switch(e_.GetEventType()){
		case EventType::KeyPressed:
			GetInstance()->buttonEvents.push_back(RawButton(dynamic_cast<const KeyPressedEvent&>(e_).GetKeyCode(), true));
			break;
		case EventType::KeyReleased:
			GetInstance()->buttonEvents.push_back(RawButton(dynamic_cast<const KeyReleasedEvent&>(e_).GetKeyCode(), false));
			break;
		case EventType::MouseMoved:
			GetInstance()->axisEvents.push_back(RawAxis(AxisID::Mouse_Move_Horizontal, dynamic_cast<const MouseMovedEvent&>(e_).GetX()));
			GetInstance()->axisEvents.push_back(RawAxis(AxisID::Mouse_Move_Vertical, dynamic_cast<const MouseMovedEvent&>(e_).GetY()));
			break;
		case EventType::MouseScroll:
			GetInstance()->axisEvents.push_back(RawAxis(AxisID::Mouse_Scroll_Horizontal, dynamic_cast<const MouseScrollEvent&>(e_).GetXOffset()));
			GetInstance()->axisEvents.push_back(RawAxis(AxisID::Mouse_Scroll_Vertical, dynamic_cast<const MouseScrollEvent&>(e_).GetYOffset()));
			break;
		case EventType::MouseButtonPressed:
			GetInstance()->buttonEvents.push_back(RawButton(dynamic_cast<const MouseButtonPressedEvent&>(e_).GetButton(), true));
			break;
		case EventType::MouseButtonReleased:
			GetInstance()->buttonEvents.push_back(RawButton(dynamic_cast<const MouseButtonReleasedEvent&>(e_).GetButton(), false));
			break;
		default:
			Debug::Log("Unhandled Event Type [" + e_.GetName().GetString() + "] in Input!", Debug::Error, __FILE__, __LINE__);
			break;
	}
}
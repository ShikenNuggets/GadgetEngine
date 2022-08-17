#include "Input.h"

#include "GamepadEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "Events/EventHandler.h"
#include "Math/Math.h"

using namespace Gadget;

Input* Input::instance = nullptr;

Input::Input() : buttonEvents(), axisEvents(), buttonsDown(), buttonsUp(), buttonsHeld(), axes(), definedButtons(), definedAxes(){
	EventHandler::GetInstance()->SetEventCallback(EventType::KeyPressed, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::KeyReleased, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseMoved, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseScroll, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseButtonPressed, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseButtonReleased, OnEvent);

	//These reserve values are somewhat arbitrary, adjust as you see fit
	buttonEvents.reserve(256);
	axisEvents.reserve(256);

	definedButtons.reserve(32);
	definedAxes.reserve(16);
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

bool Input::GetButtonDown(StringID buttonName_) const{
	for(const auto& b : definedButtons){
		if(b.GetName() == buttonName_){
			for(const auto& id : b.GetButtonIDs()){
				if(GetButtonDown(id)){
					return true;
				}
			}

			return false;
		}
	}

	Debug::Log("Tried to query undefined button [" + buttonName_.GetString() + "]!", Debug::Warning, __FILE__, __LINE__);
	return false;
}

bool Input::GetButtonUp(ButtonID id_) const{
	return buttonsUp.find(id_) != buttonsUp.end();
}

//TODO - This is almost identical to the GetButtonDown function, maybe there's a better way to reuse code here?
bool Input::GetButtonUp(StringID buttonName_) const{
	for(const auto& b : definedButtons){
		if(b.GetName() == buttonName_){
			for(const auto& id : b.GetButtonIDs()){
				if(GetButtonUp(id)){
					return true;
				}
			}

			return false;
		}
	}

	Debug::Log("Tried to query undefined button [" + buttonName_.GetString() + "]!", Debug::Warning, __FILE__, __LINE__);
	return false;
}

bool Input::GetButtonHeld(ButtonID id_) const{
	return buttonsHeld.find(id_) != buttonsHeld.end();
}

//TODO - This is almost identical to the GetButtonDown and GetButtonUp functions, maybe there's a better way to reuse code here?
bool Input::GetButtonHeld(StringID buttonName_) const{
	for(const auto& b : definedButtons){
		if(b.GetName() == buttonName_){
			for(const auto& id : b.GetButtonIDs()){
				if(GetButtonHeld(id)){
					return true;
				}
			}

			return false;
		}
	}

	Debug::Log("Tried to query undefined button [" + buttonName_.GetString() + "]!", Debug::Warning, __FILE__, __LINE__);
	return false;
}

float Input::GetAxis(AxisID id_) const{
	auto it = axes.find(id_);
	if(it != axes.end()){
		return axes.at(id_);
	}

	return 0.0f;
}

float Input::GetAxis(StringID axisName_) const{
	for(const auto& a : definedAxes){
		if(a.GetName() == axisName_){
			float totalAxisValue = 0.0f;
			for(const auto& id : a.GetAxisIDs()){
				totalAxisValue += GetAxis(id);
			}

			for(const auto& b : a.GetButtonAxisIDs()){
				if(GetButtonHeld(b.negative)){
					totalAxisValue -= 1.0f;
				}

				if(GetButtonHeld(b.positive)){
					totalAxisValue += 1.0f;
				}
			}

			return Math::Clamp(-1.0f, 1.0f, totalAxisValue);
		}
	}

	Debug::Log("Tried to query undefined axis [" + axisName_.GetString() + "]!", Debug::Warning, __FILE__, __LINE__);
	return 0.0f;
}

void Input::DefineButton(const Button&& button_){
	_ASSERT(!button_.GetButtonIDs().empty()); //Creating a Defined Button with no button IDs
	if(button_.GetButtonIDs().empty()){
		Debug::Log("Attempted to define a button [" + button_.GetName().GetString() + "] with no button IDs set!", Debug::Warning, __FILE__, __LINE__);
		return;
	}

	definedButtons.push_back(button_);
}

void Input::DefineAxis(const Axis&& axis_){
	_ASSERT(!axis_.GetAxisIDs().empty()); //Creating a Defined Axis with no axis IDs
	if(axis_.GetAxisIDs().empty()){
		Debug::Log("Attempted to define an axis [" + axis_.GetName().GetString() + "] with no button IDs set!", Debug::Warning, __FILE__, __LINE__);
		return;
	}

	definedAxes.push_back(axis_);
}

//Note - The implementation here could have some weirdness when pressing the same button multiple times/shifting an axis a lot on a single frame
//I only expect this to be an issue on frames with major lag spikes, which ideally the high-level parts of the input system/game will be somewhat resilient to
//TODO - This doesn't handle key repeats correctly
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
	buttonEvents.clear();

	for(const auto& a : axisEvents){
		if(axes.find(a.GetAxisID()) != axes.end()){
			axes[a.GetAxisID()] += a.Value(); //Update our existing axis value, in case there are multiple events for the same axis (common with mouse motion)
		}else{
			axes.insert(std::make_pair(a.GetAxisID(), a.Value()));
		}
	}
	axisEvents.clear();
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
		case EventType::GamepadAxis:
			GetInstance()->axisEvents.push_back(RawAxis(dynamic_cast<const GamepadAxisEvent&>(e_).GetAxisIndex(), dynamic_cast<const GamepadAxisEvent&>(e_).GetValue()));
			break;
		case EventType::GamepadButtonPressed:
			GetInstance()->buttonEvents.push_back(RawButton(dynamic_cast<const GamepadButtonPressedEvent&>(e_).GetButton(), true));
			break;
		case EventType::GamepadButtonReleased:
			GetInstance()->buttonEvents.push_back(RawButton(dynamic_cast<const GamepadButtonPressedEvent&>(e_).GetButton(), false));
			break;
		default:
			Debug::Log("Unhandled Event Type [" + e_.GetName().GetString() + "] in Input!", Debug::Error, __FILE__, __LINE__);
			break;
	}
}
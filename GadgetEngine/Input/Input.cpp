#include "Input.h"

#include "App.h"
#include "GamepadEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "Events/EventHandler.h"
#include "Math/Math.h"

using namespace Gadget;

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

			totalAxisValue = Math::Clamp(-1.0f, 1.0f, totalAxisValue);
			if(Math::Abs(totalAxisValue) < a.GetDeadZone()){
				return 0.0f;
			}

			if(a.IsInverted()){
				totalAxisValue *= -1.0f;
			}

			return Math::Clamp(-1.0f, 1.0f, totalAxisValue);
		}
	}

	Debug::Log("Tried to query undefined axis [" + axisName_.GetString() + "]!", Debug::Warning, __FILE__, __LINE__);
	return 0.0f;
}

bool Input::GetMultiButtonDown(StringID multiButton_) const{
	for(const auto& mb : definedMultiButtons){
		if(mb.GetName() == multiButton_){
			bool anyDownThisFrame = false;

			for(const auto& id : mb.GetButtonIDs()){
				if(GetButtonDown(id)){
					anyDownThisFrame = true;
					continue;
				}

				if(!GetButtonHeld(id)){
					return false; //All button IDs must be either down or held
				}
			}

			return anyDownThisFrame;
		}
	}

	Debug::Log("Tried to query undefined MultiButton [" + multiButton_.GetString() + "]!", Debug::Warning, __FILE__, __LINE__);
	return false;
}

bool Input::GetMultiButtonUp(StringID multiButton_) const{
	for(const auto& mb : definedMultiButtons){
		if(mb.GetName() == multiButton_){
			for(const auto& id : mb.GetButtonIDs()){
				if(GetButtonUp(id)){
					return true;
				}
			}

			return false;
		}
	}

	Debug::Log("Tried to query undefined MultiButton [" + multiButton_.GetString() + "]!", Debug::Warning, __FILE__, __LINE__);
	return false;
}

bool Input::GetMultiButtonHeld(StringID multiButton_) const{
	for(const auto& mb : definedMultiButtons){
		if(mb.GetName() == multiButton_){
			for(const auto& id : mb.GetButtonIDs()){
				if(!GetButtonDown(id) || !GetButtonHeld(id)){
					return false; //All button IDs must be either down or held
				}
			}

			return true;
		}
	}

	Debug::Log("Tried to query undefined MultiButton [" + multiButton_.GetString() + "]!", Debug::Warning, __FILE__, __LINE__);
	return false;
}

void Input::DefineButton(const Button&& button_){
	GADGET_ASSERT(!button_.GetButtonIDs().empty(), "Attempted to define a button [" + button_.GetName().GetString() + "] with no button IDs set!");
	if(button_.GetButtonIDs().empty()){
		Debug::Log("Attempted to define a button [" + button_.GetName().GetString() + "] with no button IDs set!", Debug::Warning, __FILE__, __LINE__);
		return;
	}

	definedButtons.push_back(button_);
}

void Input::DefineAxis(const Axis&& axis_){
	GADGET_ASSERT(!axis_.GetAxisIDs().empty(), "Attempted to define an axis [" + axis_.GetName().GetString() + "] with no axis IDs set!");
	if(axis_.GetAxisIDs().empty()){
		Debug::Log("Attempted to define an axis [" + axis_.GetName().GetString() + "] with no axis IDs set!", Debug::Warning, __FILE__, __LINE__);
		return;
	}

	definedAxes.push_back(axis_);
}

void Input::DefineMultiButton(const MultiButton&& multiButton_){
	GADGET_ASSERT(!multiButton_.GetButtonIDs().empty(), "Attempted to define a MultiButton [" + multiButton_.GetName().GetString() + "] with no button IDs set!");
	if(multiButton_.GetButtonIDs().empty()){
		Debug::Log("Attempted to define a MultiButton [" + multiButton_.GetName().GetString() + "] with no button IDs set!", Debug::Warning, __FILE__, __LINE__);
		return;
	}

	definedMultiButtons.push_back(multiButton_);
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
			buttonsHeld.erase(b.GetButtonID());
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
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const KeyPressedEvent&>(e_).GetKeyCode(), true));
			break;
		case EventType::KeyReleased:
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const KeyReleasedEvent&>(e_).GetKeyCode(), false));
			break;
		case EventType::MouseMoved:
			App::GetInput().axisEvents.push_back(RawAxis(AxisID::Mouse_Move_Horizontal, dynamic_cast<const MouseMovedEvent&>(e_).GetX()));
			App::GetInput().axisEvents.push_back(RawAxis(AxisID::Mouse_Move_Vertical, dynamic_cast<const MouseMovedEvent&>(e_).GetY()));
			break;
		case EventType::MouseScroll:
			App::GetInput().axisEvents.push_back(RawAxis(AxisID::Mouse_Scroll_Horizontal, dynamic_cast<const MouseScrollEvent&>(e_).GetXOffset()));
			App::GetInput().axisEvents.push_back(RawAxis(AxisID::Mouse_Scroll_Vertical, dynamic_cast<const MouseScrollEvent&>(e_).GetYOffset()));
			break;
		case EventType::MouseButtonPressed:
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const MouseButtonPressedEvent&>(e_).GetButton(), true));
			break;
		case EventType::MouseButtonReleased:
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const MouseButtonReleasedEvent&>(e_).GetButton(), false));
			break;
		case EventType::GamepadAxis:
			App::GetInput().axisEvents.push_back(RawAxis(dynamic_cast<const GamepadAxisEvent&>(e_).GetAxisIndex(), dynamic_cast<const GamepadAxisEvent&>(e_).GetValue()));
			break;
		case EventType::GamepadButtonPressed:
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const GamepadButtonPressedEvent&>(e_).GetButton(), true));
			break;
		case EventType::GamepadButtonReleased:
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const GamepadButtonPressedEvent&>(e_).GetButton(), false));
			break;
		default:
			Debug::Log("Unhandled Event Type [" + e_.GetName().GetString() + "] in Input!", Debug::Error, __FILE__, __LINE__);
			break;
	}
}
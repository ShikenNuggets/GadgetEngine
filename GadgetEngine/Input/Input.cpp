#include "Input.h"

#include "App.h"
#include "Window.h"
#include "Events/EventHandler.h"
#include "Input/GamepadEvent.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"
#include "Math/Math.h"

using namespace Gadget;

Input::Input() : buttonEvents(), axisEvents(), persistentAxisEvents(), buttonsDown(), buttonsUp(), buttonsHeld(), axes(), persistentAxes(), definedButtons(), definedAxes(), currentMouseX(0), currentMouseY(0){
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowRestarted, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::KeyPressed, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::KeyReleased, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseMoved, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseScroll, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseButtonPressed, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseButtonReleased, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::GamepadAxis, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::GamepadButtonPressed, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::GamepadButtonReleased, OnEvent);

	//These reserve values are somewhat arbitrary, adjust as you see fit
	buttonEvents.reserve(256);
	axisEvents.reserve(256);
	persistentAxisEvents.reserve(256);

	definedButtons.reserve(32);
	definedAxes.reserve(16);
}

Input::~Input(){}

bool Input::GetButtonDown(ButtonID id_) const{
	GADGET_BASIC_ASSERT(id_ < ButtonID::ButtonID_MAX);

	return buttonsDown.find(id_) != buttonsDown.end();
}

bool Input::GetButtonDown(StringID buttonName_) const{
	GADGET_BASIC_ASSERT(buttonName_ != StringID::None);
	GADGET_ASSERT(!definedButtons.empty(), "GetButtonDown called when no buttons are defined!");

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
	GADGET_BASIC_ASSERT(id_ < ButtonID::ButtonID_MAX);

	return buttonsUp.find(id_) != buttonsUp.end();
}

//TODO - This is almost identical to the GetButtonDown function, maybe there's a better way to reuse code here?
bool Input::GetButtonUp(StringID buttonName_) const{
	GADGET_BASIC_ASSERT(buttonName_ != StringID::None);
	GADGET_ASSERT(!definedButtons.empty(), "GetButtonUp called when no buttons are defined!");

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
	GADGET_BASIC_ASSERT(id_ < ButtonID::ButtonID_MAX);

	return buttonsHeld.find(id_) != buttonsHeld.end();
}

//TODO - This is almost identical to the GetButtonDown and GetButtonUp functions, maybe there's a better way to reuse code here?
bool Input::GetButtonHeld(StringID buttonName_) const{
	GADGET_BASIC_ASSERT(buttonName_ != StringID::None);
	GADGET_ASSERT(!definedButtons.empty(), "GetButtonHeld called when no buttons are defined!");

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
	GADGET_BASIC_ASSERT(id_ < AxisID::AxisID_MAX);

	auto it = axes.find(id_);
	if(it != axes.end()){
		return axes.at(id_);
	}

	it = persistentAxes.find(id_);
	if(it != persistentAxes.end()){
		return persistentAxes.at(id_);
	}

	return 0.0f;
}

float Input::GetAxis(StringID axisName_) const{
	GADGET_BASIC_ASSERT(axisName_ != StringID::None);

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

float Input::GetCurrentMouseXInGUICoordinates() const{
	GADGET_BASIC_ASSERT(App::GetRenderer().GetWindow().lock() != nullptr);
	GADGET_BASIC_ASSERT(App::GetRenderer().GetWindow().lock()->GetWidth() > 0);

	float value = static_cast<float>(currentMouseX) / static_cast<float>(App::GetRenderer().GetWindow().lock()->GetWidth());
	return Math::RemapRange(value, 0.0f, 1.0f, -1.0f, 1.0f); //Remap to a -1 to 1 range
}

float Input::GetCurrentMouseYInGUICoordinates() const{
	GADGET_BASIC_ASSERT(App::GetRenderer().GetWindow().lock() != nullptr);
	GADGET_BASIC_ASSERT(App::GetRenderer().GetWindow().lock()->GetHeight() > 0);

	float value = static_cast<float>(currentMouseY) / static_cast<float>(App::GetRenderer().GetWindow().lock()->GetHeight());
	return -Math::RemapRange(value, 0.0f, 1.0f, -1.0f, 1.0f); //Remap to a -1 to 1 range, then invert
}

bool Input::GetMultiButtonDown(StringID multiButton_) const{
	GADGET_BASIC_ASSERT(multiButton_ != StringID::None);
	GADGET_ASSERT(!definedMultiButtons.empty(), "GetMultiButtonDown called when no MultiButtons are defined!");

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
	GADGET_BASIC_ASSERT(multiButton_ != StringID::None);
	GADGET_ASSERT(!definedMultiButtons.empty(), "GetMultiButtonUp called when no MultiButtons are defined!");

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
	GADGET_BASIC_ASSERT(multiButton_ != StringID::None);
	GADGET_ASSERT(!definedMultiButtons.empty(), "GetMultiButtonHeld called when no MultiButtons are defined!");

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

void Input::DefineButton(const Button& button_){
	GADGET_ASSERT(!button_.GetButtonIDs().empty(), "Attempted to define a button [" + button_.GetName().GetString() + "] with no button IDs set!");
	if(button_.GetButtonIDs().empty()){
		Debug::Log("Attempted to define a button [" + button_.GetName().GetString() + "] with no button IDs set!", Debug::Warning, __FILE__, __LINE__);
		return;
	}

	definedButtons.push_back(button_);
}

void Input::DefineAxis(const Axis& axis_){
	GADGET_ASSERT(!axis_.GetAxisIDs().empty(), "Attempted to define an axis [" + axis_.GetName().GetString() + "] with no axis IDs set!");
	if(axis_.GetAxisIDs().empty()){
		Debug::Log("Attempted to define an axis [" + axis_.GetName().GetString() + "] with no axis IDs set!", Debug::Warning, __FILE__, __LINE__);
		return;
	}

	definedAxes.push_back(axis_);
}

void Input::DefineMultiButton(const MultiButton& multiButton_){
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

	for(const auto& a : persistentAxisEvents){
		if(persistentAxes.find(a.GetAxisID()) != persistentAxes.end()){
			persistentAxes[a.GetAxisID()] = a.Value(); //Persistent axis has a single set value
		}else{
			persistentAxes.insert(std::make_pair(a.GetAxisID(), a.Value()));
		}
	}
	persistentAxes.clear();

	GADGET_BASIC_ASSERT(buttonEvents.empty());
	GADGET_BASIC_ASSERT(axisEvents.empty());
	GADGET_BASIC_ASSERT(persistentAxes.empty());
}

void Input::OnEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() < EventType::Count);
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	switch(e_.GetEventType()){
		case EventType::WindowRestarted:
			//Clear everything on a window restart
			App::GetInput().axisEvents.clear();
			App::GetInput().buttonEvents.clear();
			App::GetInput().persistentAxisEvents.clear();
			App::GetInput().buttonsDown.clear();
			App::GetInput().buttonsHeld.clear();
			App::GetInput().buttonsUp.clear();
			App::GetInput().axes.clear();
			App::GetInput().persistentAxes.clear();
			break;
		case EventType::KeyPressed:
			GADGET_BASIC_ASSERT(dynamic_cast<const KeyPressedEvent*>(&e_) != nullptr);
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const KeyPressedEvent&>(e_).GetKeyCode(), true));
			break;
		case EventType::KeyReleased:
			GADGET_BASIC_ASSERT(dynamic_cast<const KeyReleasedEvent*>(&e_) != nullptr);
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const KeyReleasedEvent&>(e_).GetKeyCode(), false));
			break;
		case EventType::MouseMoved:
			GADGET_BASIC_ASSERT(dynamic_cast<const MouseMovedEvent*>(&e_) != nullptr);
			App::GetInput().currentMouseX = static_cast<int>(dynamic_cast<const MouseMovedEvent&>(e_).GetXAbsolute());
			App::GetInput().currentMouseY = static_cast<int>(dynamic_cast<const MouseMovedEvent&>(e_).GetYAbsolute());

			App::GetInput().axisEvents.push_back(RawAxis(AxisID::Mouse_Move_Horizontal, dynamic_cast<const MouseMovedEvent&>(e_).GetX()));
			App::GetInput().axisEvents.push_back(RawAxis(AxisID::Mouse_Move_Vertical, dynamic_cast<const MouseMovedEvent&>(e_).GetY()));
			break;
		case EventType::MouseScroll:
			GADGET_BASIC_ASSERT(dynamic_cast<const MouseScrollEvent*>(&e_) != nullptr);
			App::GetInput().axisEvents.push_back(RawAxis(AxisID::Mouse_Scroll_Horizontal, dynamic_cast<const MouseScrollEvent&>(e_).GetXOffset()));
			App::GetInput().axisEvents.push_back(RawAxis(AxisID::Mouse_Scroll_Vertical, dynamic_cast<const MouseScrollEvent&>(e_).GetYOffset()));
			break;
		case EventType::MouseButtonPressed:
			GADGET_BASIC_ASSERT(dynamic_cast<const MouseButtonPressedEvent*>(&e_) != nullptr);
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const MouseButtonPressedEvent&>(e_).GetButton(), true));
			break;
		case EventType::MouseButtonReleased:
			GADGET_BASIC_ASSERT(dynamic_cast<const MouseButtonReleasedEvent*>(&e_) != nullptr);
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const MouseButtonReleasedEvent&>(e_).GetButton(), false));
			break;
		case EventType::GamepadAxis:
			GADGET_BASIC_ASSERT(dynamic_cast<const GamepadAxisEvent*>(&e_) != nullptr);
			App::GetInput().persistentAxisEvents.push_back(RawAxis(dynamic_cast<const GamepadAxisEvent&>(e_).GetAxisIndex(), dynamic_cast<const GamepadAxisEvent&>(e_).GetValue()));
			break;
		case EventType::GamepadButtonPressed:
			GADGET_BASIC_ASSERT(dynamic_cast<const GamepadButtonPressedEvent*>(&e_) != nullptr);
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const GamepadButtonPressedEvent&>(e_).GetButton(), true));
			break;
		case EventType::GamepadButtonReleased:
			GADGET_BASIC_ASSERT(dynamic_cast<const GamepadButtonReleasedEvent*>(&e_) != nullptr);
			App::GetInput().buttonEvents.push_back(RawButton(dynamic_cast<const GamepadButtonReleasedEvent&>(e_).GetButton(), false));
			break;
		default:
			Debug::Log("Unhandled Event Type [" + e_.GetName().GetString() + "] in Input!", Debug::Error, __FILE__, __LINE__);
			break;
	}
}
#include "Input.h"

#include "App.h"
#include "Window.h"
#include "Events/AppEvent.h"
#include "Events/EventHandler.h"
#include "Input/GamepadEvent.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"
#include "Math/Math.h"

using namespace Gadget;

//These reserve values are somewhat arbitrary, adjust as you see fit
static constexpr size_t gNumEventsToReserve = 256;
static constexpr size_t gNumButtonsToReserve = 32;
static constexpr size_t gNumAxesToReserve = 16;

Input::Input() : buttonEvents(), axisEvents(), persistentAxisEvents(), buttonsDown(), buttonsHeld(), buttonsUp(), axes(), persistentAxes(), currentMouseX(0), currentMouseY(0), definedButtons(), definedAxes(){
	EventHandler::GetInstance()->RegisterCallback(EventType::WindowRestarted,		this,	[&](const Event& e){ OnWindowRestartedEvent(e); });
	EventHandler::GetInstance()->RegisterCallback(EventType::KeyPressed,			this,	[&](const Event& e){ OnKeyPressedEvent(e); });
	EventHandler::GetInstance()->RegisterCallback(EventType::KeyReleased,			this,	[&](const Event& e){ OnKeyReleasedEvent(e); });
	EventHandler::GetInstance()->RegisterCallback(EventType::MouseMoved,			this,	[&](const Event& e){ OnMouseMovedEvent(e); });
	EventHandler::GetInstance()->RegisterCallback(EventType::MouseScroll,			this,	[&](const Event& e){ OnMouseScrollEvent(e); });
	EventHandler::GetInstance()->RegisterCallback(EventType::MouseButtonPressed,	this,	[&](const Event& e){ OnMouseButtonPressedEvent(e); });
	EventHandler::GetInstance()->RegisterCallback(EventType::MouseButtonReleased,	this,	[&](const Event& e){ OnMouseButtonReleasedEvent(e); });
	EventHandler::GetInstance()->RegisterCallback(EventType::GamepadAxis,			this,	[&](const Event& e){ OnGamepadAxisEvent(e); });
	EventHandler::GetInstance()->RegisterCallback(EventType::GamepadButtonPressed,	this,	[&](const Event& e){ OnGamepadButtonPressedEvent(e); });
	EventHandler::GetInstance()->RegisterCallback(EventType::GamepadButtonReleased,	this,	[&](const Event& e){ OnGamepadButtonReleasedEvent(e); });
	
	buttonEvents.reserve(gNumEventsToReserve);
	axisEvents.reserve(gNumEventsToReserve);
	persistentAxisEvents.reserve(gNumEventsToReserve);

	definedButtons.reserve(gNumButtonsToReserve);
	definedAxes.reserve(gNumAxesToReserve);
}

Input::~Input(){
	EventHandler::GetInstance()->UnregisterCallbacks(this);
}

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

bool Input::GetAnyButtonDown() const{
	return !buttonsDown.empty();
}

bool Input::GetAnyButtonDown(ButtonID ignore_) const{
	for(const auto& b : buttonsDown){
		if(b != ignore_){
			return true;
		}
	}

	return false;
}

bool Input::GetAnyButtonDown(StringID ignoreButtonName_) const{
	for(const auto& b : definedButtons){
		if(ignoreButtonName_ != b.GetName() && GetButtonDown(b.GetName())){
			return true;
		}
	}

	return false;
}

bool Input::GetAnyButtonUp() const{
	return !buttonsUp.empty();
}

bool Input::GetAnyButtonUp(ButtonID ignore_) const{
	for(const auto& b : buttonsUp){
		if(b != ignore_){
			return true;
		}
	}

	return false;
}

bool Input::GetAnyButtonUp(StringID ignoreButtonName_) const{
	for(const auto& b : definedButtons){
		if(ignoreButtonName_ != b.GetName() && GetButtonUp(b.GetName())){
			return true;
		}
	}

	return false;
}

bool Input::GetAnyButtonHeld() const{
	return !buttonsHeld.empty();
}

bool Input::GetAnyButtonHeld(ButtonID ignore_) const{
	for(const auto& b : buttonsHeld){
		if(b != ignore_){
			return true;
		}
	}

	return false;
}

bool Input::GetAnyButtonHeld(StringID ignoreButtonName_) const{
	for(const auto& b : definedButtons){
		if(ignoreButtonName_ != b.GetName() && GetButtonHeld(b.GetName())){
			return true;
		}
	}

	return false;
}

float Input::GetAxis(AxisID id_) const{
	GADGET_BASIC_ASSERT(id_ < AxisID::AxisID_MAX);

	if(axes.Contains(id_)){
		return axes[id_];
	}

	if(persistentAxes.Contains(id_)){
		return persistentAxes[id_];
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

	const float value = static_cast<float>(currentMouseX) / static_cast<float>(App::GetRenderer().GetWindow().lock()->GetWidth());
	return Math::RemapRange(value, 0.0f, 1.0f, -1.0f, 1.0f); //Remap to a -1 to 1 range
}

float Input::GetCurrentMouseYInGUICoordinates() const{
	GADGET_BASIC_ASSERT(App::GetRenderer().GetWindow().lock() != nullptr);
	GADGET_BASIC_ASSERT(App::GetRenderer().GetWindow().lock()->GetHeight() > 0);

	const float value = static_cast<float>(currentMouseY) / static_cast<float>(App::GetRenderer().GetWindow().lock()->GetHeight());
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
	axes.Clear();

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
		if(axes.Contains(a.GetAxisID())){
			axes[a.GetAxisID()] += a.Value(); //Update our existing axis value, in case there are multiple events for the same axis (common with mouse motion)
		}else{
			axes.Add(a.GetAxisID(), a.Value());
		}
	}
	axisEvents.clear();

	for(const auto& a : persistentAxisEvents){
		if(persistentAxes.Contains(a.GetAxisID())){
			persistentAxes[a.GetAxisID()] = a.Value(); //Persistent axis has a single set value
		}else{
			persistentAxes.Add(a.GetAxisID(), a.Value());
		}
	}
	persistentAxes.Clear();

	GADGET_BASIC_ASSERT(buttonEvents.empty());
	GADGET_BASIC_ASSERT(axisEvents.empty());
	GADGET_BASIC_ASSERT(persistentAxes.IsEmpty());
}

void Input::OnWindowRestartedEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == WindowRestartedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	axisEvents.clear();
	buttonEvents.clear();
	persistentAxisEvents.clear();
	buttonsDown.clear();
	buttonsHeld.clear();
	buttonsUp.clear();
	axes.Clear();
	persistentAxes.Clear();
}

void Input::OnKeyPressedEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == KeyPressedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const KeyPressedEvent* eventPtr = dynamic_cast<const KeyPressedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		buttonEvents.emplace_back(eventPtr->GetKeyCode(), true);
	}
}

void Input::OnKeyReleasedEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == KeyReleasedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const KeyReleasedEvent* eventPtr = dynamic_cast<const KeyReleasedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		buttonEvents.emplace_back(eventPtr->GetKeyCode(), false);
	}
}

void Input::OnMouseMovedEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == MouseMovedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const MouseMovedEvent* eventPtr = dynamic_cast<const MouseMovedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		currentMouseX = static_cast<int>(eventPtr->GetXAbsolute());
		currentMouseY = static_cast<int>(eventPtr->GetYAbsolute());

		axisEvents.emplace_back(AxisID::Mouse_Move_Horizontal, eventPtr->GetX());
		axisEvents.emplace_back(AxisID::Mouse_Move_Vertical, eventPtr->GetY());
	}
}

void Input::OnMouseScrollEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == MouseScrollEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const MouseScrollEvent* eventPtr = dynamic_cast<const MouseScrollEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		axisEvents.emplace_back(AxisID::Mouse_Scroll_Horizontal, eventPtr->GetXOffset());
		axisEvents.emplace_back(AxisID::Mouse_Scroll_Vertical, eventPtr->GetYOffset());
	}
}

void Input::OnMouseButtonPressedEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == MouseButtonPressedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const MouseButtonPressedEvent* eventPtr = dynamic_cast<const MouseButtonPressedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		buttonEvents.emplace_back(eventPtr->GetButton(), true);
	}
}

void Input::OnMouseButtonReleasedEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == MouseButtonReleasedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const MouseButtonReleasedEvent* eventPtr = dynamic_cast<const MouseButtonReleasedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		buttonEvents.emplace_back(eventPtr->GetButton(), false);
	}
}

void Input::OnGamepadAxisEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == GamepadAxisEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const GamepadAxisEvent* eventPtr = dynamic_cast<const GamepadAxisEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		persistentAxisEvents.emplace_back(eventPtr->GetAxisIndex(), eventPtr->GetValue());
	}
}

void Input::OnGamepadButtonPressedEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == GamepadButtonPressedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const GamepadButtonPressedEvent* eventPtr = dynamic_cast<const GamepadButtonPressedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		buttonEvents.emplace_back(eventPtr->GetButton(), true);
	}
}

void Input::OnGamepadButtonReleasedEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == GamepadButtonReleasedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const GamepadButtonReleasedEvent* eventPtr = dynamic_cast<const GamepadButtonReleasedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		buttonEvents.emplace_back(eventPtr->GetButton(), false);
	}
}
#include "InputGesture.h"

#include "Input.h"
#include "Core/Time.h"
#include "Math/Math.h"

using namespace Gadget;

InputGesture::InputGesture(StringID name_, float maxAllowedTime_) : name(name_), maxAllowedTime(maxAllowedTime_){}

StringID InputGesture::GetName() const{ return name; }

ButtonMashGesture::ButtonMashGesture(StringID name_, ButtonID button_, int requiredPresses_, float maxAllowedTime_) : InputGesture(name_, maxAllowedTime_), buttonIDs(), requiredPresses(requiredPresses_), currentPresses(0){
	buttonIDs.push_back(button_);
	lastButtonDownEvent = Time::GetInstance()->TimeSinceStartup() - maxAllowedTime; //Start as invalid
}

ButtonMashGesture::ButtonMashGesture(StringID name_, std::vector<ButtonID> buttonIDs_, int requiredPresses_, float maxAllowedTime_) : InputGesture(name_, maxAllowedTime_), buttonIDs(buttonIDs_), requiredPresses(requiredPresses_), currentPresses(0){
	lastButtonDownEvent = Time::GetInstance()->TimeSinceStartup() - maxAllowedTime; //Start as invalid
}

bool ButtonMashGesture::IsGestureValid() const{
	return (Time::GetInstance()->TimeSinceStartup() - lastButtonDownEvent) < maxAllowedTime;
}

bool ButtonMashGesture::IsGestureComplete() const{
	return currentPresses >= requiredPresses;
}

float ButtonMashGesture::GetPercentComplete() const{
	_ASSERT(requiredPresses > 0);
	return Math::Clamp(0.0f, 1.0f, static_cast<float>(currentPresses) / static_cast<float>(requiredPresses));
}

void ButtonMashGesture::Update(){
	for(const auto& b : buttonIDs){
		if(Input::GetInstance()->GetButtonDown(b)){
			currentPresses++;
			lastButtonDownEvent = Time::GetInstance()->TimeSinceStartup();
			break;
		}
	}

	if(!IsGestureValid()){
		Reset();
	}
}

void ButtonMashGesture::Reset(){
	currentPresses = 0;
}

ButtonSequenceGesture::ButtonSequenceGesture(StringID name_, std::vector<ButtonID> sequence_, float maxAllowedTime_) : InputGesture(name_, maxAllowedTime_), buttonIDs(), startTime(0.0f), currentButtonIndex(0){
	buttonIDs.push_back(sequence_);
}

ButtonSequenceGesture::ButtonSequenceGesture(StringID name_, std::vector<std::vector<ButtonID>> sequences_, float maxAllowedTime_) : InputGesture(name_, maxAllowedTime_), buttonIDs(sequences_), startTime(0.0f), currentButtonIndex(0){
	_ASSERT(!sequences_.empty()); //Tried to create an empty button sequence gesture

	#ifdef GADGET_DEBUG
	#pragma warning(disable : 4189) //Kill useless warning about unused variable
	for(const auto& sequence : buttonIDs){
		_ASSERT(sequence.size() == buttonIDs[0].size());
	}
	#pragma warning(default : 4189)
	#endif
}

bool ButtonSequenceGesture::IsGestureValid() const{
	if(startTime == 0.0f){
		return false;
	}

	return (Time::GetInstance()->TimeSinceStartup() - startTime) < maxAllowedTime;
}

bool ButtonSequenceGesture::IsGestureComplete() const{
	return currentButtonIndex == buttonIDs[0].size();
}

void ButtonSequenceGesture::Update(){
	_ASSERT(!buttonIDs.empty());

	if(currentButtonIndex >= buttonIDs[0].size()){
		return; //The sequence is already complete
	}

	for(const auto& s : buttonIDs){
		if(Input::GetInstance()->GetButtonDown(s[currentButtonIndex])){
			if(currentButtonIndex == 0){
				startTime = Time::GetInstance()->TimeSinceStartup();
			}

			if((Time::GetInstance()->TimeSinceStartup() - startTime) < maxAllowedTime){
				currentButtonIndex++;

			}else{
				Reset();
			}

			break;
		}
	}
}

void ButtonSequenceGesture::Reset(){
	startTime = 0.0f;
	currentButtonIndex = 0;
}
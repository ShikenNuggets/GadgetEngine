#include "InputGesture.h"

#include "App.h"
#include "Core/Time.h"
#include "Input/Input.h"
#include "Math/Math.h"

using namespace Gadget;

InputGesture::InputGesture(StringID name_, float maxAllowedTime_) : name(name_), maxAllowedTime(maxAllowedTime_){
	GADGET_BASIC_ASSERT(name_ != StringID::None);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(maxAllowedTime_));
	GADGET_BASIC_ASSERT(maxAllowedTime_ > 0.0f);
}

StringID InputGesture::GetName() const{ return name; }

//---------Button Mash Gesture----------//
ButtonMashGesture::ButtonMashGesture(StringID gestureName_, StringID buttonName_, int requiredPresses_, float maxAllowedTime_) : InputGesture(gestureName_, maxAllowedTime_), buttonName(buttonName_), requiredPresses(requiredPresses_), currentPresses(0), lastButtonDownEvent(0.0f){
	GADGET_BASIC_ASSERT(gestureName_ != StringID::None);
	GADGET_BASIC_ASSERT(buttonName_ != StringID::None);
	GADGET_BASIC_ASSERT(requiredPresses_ > 0);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(maxAllowedTime_));
	GADGET_BASIC_ASSERT(maxAllowedTime_ > 0.0f);
}

bool ButtonMashGesture::IsGestureValid() const{
	return (App::GetTime().TimeSinceStartup() - lastButtonDownEvent) < maxAllowedTime;
}

bool ButtonMashGesture::IsGestureComplete() const{
	return currentPresses >= requiredPresses;
}

float ButtonMashGesture::GetPercentComplete() const{
	GADGET_ASSERT(requiredPresses > 0, "Buttom mash gesture with zero required presses exists!");
	return Math::Clamp(0.0f, 1.0f, static_cast<float>(currentPresses) / static_cast<float>(requiredPresses));
}

void ButtonMashGesture::Update(){
	if(IsGestureComplete()){
		return;
	}

	if(App::GetInput().GetButtonDown(buttonName)){
		currentPresses++;
		lastButtonDownEvent = App::GetTime().TimeSinceStartup();
	}

	if(!IsGestureValid()){
		Reset();
	}
}

void ButtonMashGesture::Reset(){
	currentPresses = 0;
}

//---------Button Sequence Gesture----------//
ButtonSequenceGesture::ButtonSequenceGesture(StringID name_, std::vector<StringID> sequence_, float maxAllowedTime_) : InputGesture(name_, maxAllowedTime_), buttonNames(sequence_), startTime(0.0f), currentButtonIndex(0){
	GADGET_BASIC_ASSERT(name_ != StringID::None);
	GADGET_BASIC_ASSERT(!sequence_.empty());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(maxAllowedTime_));
	GADGET_BASIC_ASSERT(maxAllowedTime_ > 0.0f);
}

bool ButtonSequenceGesture::IsGestureValid() const{
	if(startTime == 0.0f || currentButtonIndex == 0){
		return false;
	}

	return (App::GetTime().TimeSinceStartup() - startTime) < maxAllowedTime;
}

bool ButtonSequenceGesture::IsGestureComplete() const{
	return currentButtonIndex >= buttonNames.size();
}

//TODO - Pressing the wrong button should reset the gesture
void ButtonSequenceGesture::Update(){
	GADGET_ASSERT(!buttonNames.empty(), "ButtonSequenceGesture with no button names set exists!");

	if(IsGestureComplete()){
		return; //The sequence is already complete
	}

	GADGET_BASIC_ASSERT(currentButtonIndex < buttonNames.size());

	if(App::GetInput().GetButtonDown(buttonNames[currentButtonIndex])){
		if(currentButtonIndex == 0){
			startTime = App::GetTime().TimeSinceStartup();
		}

		if(IsGestureValid()){
			currentButtonIndex++;
		}else{
			Reset();
		}
	}
}

void ButtonSequenceGesture::Reset(){
	startTime = 0.0f;
	currentButtonIndex = 0;
}

//---------Circular Rotation Gesture----------//
CircularRotationGesture::CircularRotationGesture(StringID gestureName_, StringID horizontalAxisName_, StringID verticalAxisName_, std::vector<Rotations> rotations_, float maxAllowedTime_) : InputGesture(gestureName_, maxAllowedTime_), horizontal(horizontalAxisName_), vertical(verticalAxisName_), rotations(rotations_), startTime(0.0f), currentRotationIndex(0){
	GADGET_BASIC_ASSERT(gestureName_ != StringID::None);
	GADGET_BASIC_ASSERT(horizontalAxisName_ != StringID::None);
	GADGET_BASIC_ASSERT(verticalAxisName_ != StringID::None);
	GADGET_ASSERT(!rotations.empty(), "CircularRotationGesture created with no rotations set!");
	GADGET_BASIC_ASSERT(Math::IsValidNumber(maxAllowedTime_));
	GADGET_BASIC_ASSERT(maxAllowedTime_ > 0.0f);
}

bool CircularRotationGesture::IsGestureValid() const{
	return (App::GetTime().TimeSinceStartup() - startTime) < maxAllowedTime;
}

bool CircularRotationGesture::IsGestureComplete() const{
	return currentRotationIndex >= rotations.size();
}

void CircularRotationGesture::Update(){
	GADGET_ASSERT(!rotations.empty(), "CircularRotationGesture with no rotations set exists!");

	if(IsGestureComplete()){
		return;
	}

	GADGET_BASIC_ASSERT(currentRotationIndex < rotations.size());

	float xRot = App::GetInput().GetAxis(horizontal);
	float yRot = App::GetInput().GetAxis(vertical);

	switch(rotations[currentRotationIndex]){
		case Rotations::UpLeft:
			if(xRot < 0.0f && yRot > 0.0f){
				currentRotationIndex++;
			}else if(xRot != 0.0f && yRot != 0.0f){
				Reset();
			}
			break;

		case Rotations::UpRight:
			if(xRot > 0.0f && yRot > 0.0f){
				currentRotationIndex++;
			}else if(xRot != 0.0f && yRot != 0.0f){
				Reset();
			}
			break;

		case Rotations::DownLeft:
			if(xRot < 0.0f && yRot < 0.0f){
				currentRotationIndex++;
			}else if(xRot != 0.0f && yRot != 0.0f){
				Reset();
			}
			break;

		case Rotations::DownRight:
			if(xRot > 0.0f && yRot < 0.0f){
				currentRotationIndex++;
			}else if(xRot != 0.0f && yRot != 0.0f){
				Reset();
			}
			break;

		default:
			Debug::Log("Unhandled Rotation value!", Debug::Error, __FILE__, __LINE__);
			break;
	}

	if(currentRotationIndex == 1){
		startTime = App::GetTime().TimeSinceStartup();
	}

	if(!IsGestureValid()){
		Reset();
	}
}

void CircularRotationGesture::Reset(){
	startTime = 0.0f;
	currentRotationIndex = 0;
}
#include "InputGesture.h"

#include "Input.h"
#include "Core/Time.h"
#include "Math/Math.h"

using namespace Gadget;

InputGesture::InputGesture(StringID name_, float maxAllowedTime_) : name(name_), maxAllowedTime(maxAllowedTime_){}

StringID InputGesture::GetName() const{ return name; }

//---------Button Mash Gesture----------//
ButtonMashGesture::ButtonMashGesture(StringID gestureName_, StringID buttonName_, int requiredPresses_, float maxAllowedTime_) : InputGesture(gestureName_, maxAllowedTime_), buttonName(buttonName_), requiredPresses(requiredPresses_), currentPresses(0), lastButtonDownEvent(0.0f){}

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
	if(IsGestureComplete()){
		return;
	}

	if(Input::GetInstance()->GetButtonDown(buttonName)){
		currentPresses++;
		lastButtonDownEvent = Time::GetInstance()->TimeSinceStartup();
	}

	if(!IsGestureValid()){
		Reset();
	}
}

void ButtonMashGesture::Reset(){
	currentPresses = 0;
}

//---------Button Sequence Gesture----------//
ButtonSequenceGesture::ButtonSequenceGesture(StringID name_, std::vector<StringID> sequence_, float maxAllowedTime_) : InputGesture(name_, maxAllowedTime_), buttonNames(sequence_), startTime(0.0f), currentButtonIndex(0){}

bool ButtonSequenceGesture::IsGestureValid() const{
	if(startTime == 0.0f || currentButtonIndex == 0){
		return false;
	}

	return (Time::GetInstance()->TimeSinceStartup() - startTime) < maxAllowedTime;
}

bool ButtonSequenceGesture::IsGestureComplete() const{
	return currentButtonIndex >= buttonNames.size();
}

//TODO - Pressing the wrong button should reset the gesture
void ButtonSequenceGesture::Update(){
	_ASSERT(!buttonNames.empty());

	if(IsGestureComplete()){
		return; //The sequence is already complete
	}

	if(Input::GetInstance()->GetButtonDown(buttonNames[currentButtonIndex])){
		if(currentButtonIndex == 0){
			startTime = Time::GetInstance()->TimeSinceStartup();
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
	_ASSERT(!rotations.empty());
}

bool CircularRotationGesture::IsGestureValid() const{
	return (Time::GetInstance()->TimeSinceStartup() - startTime) < maxAllowedTime;
}

bool CircularRotationGesture::IsGestureComplete() const{
	return currentRotationIndex >= rotations.size();
}

void CircularRotationGesture::Update(){
	_ASSERT(!rotations.empty());

	if(IsGestureComplete()){
		return;
	}

	float xRot = Input::GetInstance()->GetAxis(horizontal);
	float yRot = Input::GetInstance()->GetAxis(vertical);

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
		startTime = Time::GetInstance()->TimeSinceStartup();
	}

	if(!IsGestureValid()){
		Reset();
	}
}

void CircularRotationGesture::Reset(){
	startTime = 0.0f;
	currentRotationIndex = 0;
}
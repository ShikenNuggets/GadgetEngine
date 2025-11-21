#ifndef GADGET_INPUT_GESTURE_H
#define GADGET_INPUT_GESTURE_H

#include "Input/InputType.h"
#include "Utils/StringID.h"

namespace Gadget{
	class InputGesture{
	public:
		InputGesture(StringID name_, float maxAllowedTime_);
		virtual ~InputGesture(){}

		StringID GetName() const;
		virtual bool IsGestureValid() const = 0;
		virtual bool IsGestureComplete() const = 0;

		//To be called once per frame
		virtual void Update() = 0;
		virtual void Reset() = 0;

	protected:
		StringID name;
		float maxAllowedTime;
	};

	class ButtonMashGesture : public InputGesture{
	public:
		ButtonMashGesture(StringID gestureName_, StringID buttonName_, int requiredPresses_ = 25, float maxAllowedTime_ = 0.25f);
		virtual ~ButtonMashGesture(){}

		virtual bool IsGestureValid() const override;
		virtual bool IsGestureComplete() const override;
		//Returns a value between 0 and 1
		float GetPercentComplete() const;

		virtual void Update() override;
		virtual void Reset() override;

	private:
		StringID buttonName;
		int requiredPresses;
		int currentPresses;
		float lastButtonDownEvent;
	};

	class ButtonSequenceGesture : public InputGesture{
	public:
		ButtonSequenceGesture(StringID gestureName_, const std::vector<StringID>& sequence_, float maxAllowedTime_ = 1.0f);
		virtual ~ButtonSequenceGesture(){}

		virtual bool IsGestureValid() const override;
		virtual bool IsGestureComplete() const override;
		
		virtual void Update() override;
		virtual void Reset() override;

	private:
		std::vector<StringID> buttonNames;
		float startTime;
		size_t currentButtonIndex;
	};

	class CircularRotationGesture : public InputGesture{
	public:
		enum class Rotations{
			UpLeft,
			UpRight,
			DownLeft,
			DownRight
		};

		CircularRotationGesture(StringID gestureName_, StringID horizontalAxisName_, StringID verticalAxisName_, const std::vector<Rotations>& rotations_, float maxAllowedTime_ = 1.0f);
		virtual ~CircularRotationGesture(){}

		virtual bool IsGestureValid() const override;
		virtual bool IsGestureComplete() const override;

		virtual void Update() override;
		virtual void Reset() override;

	private:
		StringID horizontal;
		StringID vertical;
		std::vector<Rotations> rotations;
		float startTime;
		size_t currentRotationIndex;
	};
}

#endif //!GADGET_INPUT_GESTURE_H
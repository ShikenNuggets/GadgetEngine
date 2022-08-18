#ifndef GADGET_INPUT_GESTURE_H
#define GADGET_INPUT_GESTURE_H

#include "Input/InputEnums.h"
#include "Utils/StringID.h"

namespace Gadget{
	class InputGesture{
	public:
		InputGesture(StringID name_, float maxAllowedTime_);

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
		ButtonMashGesture(StringID name_, ButtonID button_, int requiredPresses_ = 25, float maxAllowedTime_ = 0.25f);
		ButtonMashGesture(StringID name_, std::vector<ButtonID> buttonIDs_, int requiredPresses_ = 25, float maxAllowedTime_ = 0.25f);

		virtual bool IsGestureValid() const override;
		virtual bool IsGestureComplete() const override;
		//Returns a value between 0 and 1
		float GetPercentComplete() const;

		virtual void Update() override;
		virtual void Reset() override;

	private:
		std::vector<ButtonID> buttonIDs;
		int requiredPresses;
		int currentPresses;
		float lastButtonDownEvent;
	};

	class ButtonSequenceGesture : public InputGesture{
	public:
		ButtonSequenceGesture(StringID name_, std::vector<ButtonID> sequence_, float maxAllowedTime_ = 1.0f);
		ButtonSequenceGesture(StringID name_, std::vector<std::vector<ButtonID>> sequences_, float maxAllowedTime_ = 1.0f);

		virtual bool IsGestureValid() const override;
		virtual bool IsGestureComplete() const override;
		
		virtual void Update() override;
		virtual void Reset() override;

	private:
		std::vector<std::vector<ButtonID>> buttonIDs; //A list (vector) of sequences (vector)
		float startTime;
		size_t currentButtonIndex;
	};
}

#endif //!GADGET_INPUT_GESTURE_H
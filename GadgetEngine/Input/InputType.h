#ifndef GADGET_INPUT_TYPE_H
#define GADGET_INPUT_TYPE_H

#include "InputEnums.h"

namespace Gadget{
	class Button{
	public:
		constexpr Button(ButtonID buttonID_, bool isPressed_ = false) : buttonID(buttonID_), isPressed(isPressed_){}

		constexpr ButtonID GetButtonID() const{ return buttonID; }
		constexpr bool IsPressed() const{ return isPressed; }

	private:
		const ButtonID buttonID;
		const bool isPressed;
	};

	class Axis{
	public:
		constexpr Axis(AxisID axisID_, float value_ = 0.0f) : axisID(axisID_), value(value_){}

		constexpr AxisID GetAxisID() const{ return axisID; }
		constexpr float Value() const{ return value; }

	private:
		const AxisID axisID;
		const float value;
	};
}

#endif //!GADGET_INPUT_TYPE_H
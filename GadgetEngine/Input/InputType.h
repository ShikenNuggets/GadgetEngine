#ifndef GADGET_INPUT_TYPE_H
#define GADGET_INPUT_TYPE_H

#include "InputEnums.h"
#include "Utils/StringID.h"

namespace Gadget{
	class RawButton{
	public:
		constexpr RawButton(ButtonID buttonID_, bool isPressed_ = false) : buttonID(buttonID_), isPressed(isPressed_){}

		constexpr ButtonID GetButtonID() const{ return buttonID; }
		constexpr bool IsPressed() const{ return isPressed; }

	private:
		const ButtonID buttonID;
		const bool isPressed;
	};

	class RawAxis{
	public:
		constexpr RawAxis(AxisID axisID_, float value_ = 0.0f) : axisID(axisID_), value(value_){}

		constexpr AxisID GetAxisID() const{ return axisID; }
		constexpr float Value() const{ return value; }

	private:
		const AxisID axisID;
		const float value;
	};

	class Button{
	public:
		Button(StringID name_) : name(name_), buttonIDs(){}

		Button(StringID name_, ButtonID buttonID_) : name(name_), buttonIDs(){
			buttonIDs.insert(buttonID_);
		}

		Button(StringID name_, const std::set<ButtonID>&& buttonIDs_) : name(name_), buttonIDs(buttonIDs_){}

		StringID GetName() const{ return name; }
		const std::set<ButtonID>& GetButtonIDs() const{ return buttonIDs; }

		void AddButtonID(ButtonID id_){ buttonIDs.insert(id_); }

		void RemoveButtonID(ButtonID id_){
			_ASSERT(buttonIDs.find(id_) != buttonIDs.end()); //Tried erasing a button ID that's not in the set
			buttonIDs.erase(id_);
		}

	private:
		StringID name;
		std::set<ButtonID> buttonIDs;
	};

	struct ButtonAxis{
		constexpr ButtonAxis(ButtonID negative_, ButtonID positive_) : negative(negative_), positive(positive_){}

		ButtonID negative;
		ButtonID positive;
	};

	class Axis{
	public:
		Axis(StringID name_, AxisID axisID_) : name(name_), axisIDs(), buttonAxisIDs(){
			axisIDs.insert(axisID_);
		}

		Axis(StringID name_, const ButtonAxis& buttonAxis_) : name(name_), axisIDs(), buttonAxisIDs(){
			buttonAxisIDs.push_back(buttonAxis_);
		}

		Axis(StringID name_, const std::set<AxisID>&& axisIDs_) : name(name_), axisIDs(axisIDs_), buttonAxisIDs(){}
		Axis(StringID name_, const std::vector<ButtonAxis>&& buttonAxisIDs_) : name(name_), axisIDs(), buttonAxisIDs(buttonAxisIDs_){}

		StringID GetName() const{ return name; }
		const std::set<AxisID>& GetAxisIDs() const{ return axisIDs; }
		const std::vector<ButtonAxis>& GetButtonAxisIDs() const{ return buttonAxisIDs; }

		void AddAxisID(AxisID id_){ axisIDs.insert(id_); }

		void RemoveAxisID(AxisID id_){
			_ASSERT(axisIDs.find(id_) != axisIDs.end()); //Tried erasing an axis ID that's not in the set
			axisIDs.erase(id_);
		}

	private:
		StringID name;
		std::set<AxisID> axisIDs;
		std::vector<ButtonAxis> buttonAxisIDs;
	};
}

#endif //!GADGET_INPUT_TYPE_H
#ifndef GADGET_INPUT_TYPE_H
#define GADGET_INPUT_TYPE_H

#include <set>

#include "InputEnums.h"
#include "Debug.h"
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

	//Action that requires a single button
	//If ANY of the buttonIDs is up/held/down, the Button is
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
			GADGET_ASSERT(buttonIDs.find(id_) != buttonIDs.end(), "Tried to remove invalid button ID [" + std::to_string((int)id_) + "]!");
			buttonIDs.erase(id_);
		}

	private:
		StringID name;
		std::set<ButtonID> buttonIDs;
	};

	//Action that requires multiple buttons
	//If ALL of the buttonIDs are down/held and ANY are down, the MultiButton is down
	//If ALL of the buttonIDs are held, the MultiButton is held
	//If the MultiButton was previously down and ANY of the buttonIDs are up, the MultiButton is up
	class MultiButton{
	public:
		MultiButton(StringID name_) : name(name_), buttonIDs(){}

		MultiButton(StringID name_, ButtonID buttonID_) : name(name_), buttonIDs(){
			buttonIDs.push_back(buttonID_);
		}

		MultiButton(StringID name_, const std::vector<ButtonID>&& buttonIDs_) : name(name_), buttonIDs(buttonIDs_){}

		StringID GetName() const{ return name; }
		const std::vector<ButtonID>& GetButtonIDs() const{ return buttonIDs; }

		void AddButtonID(ButtonID id_){ buttonIDs.push_back(id_); }

		void RemoveButtonID(ButtonID id_){
			GADGET_ASSERT(std::find(buttonIDs.begin(), buttonIDs.end(), id_) != buttonIDs.end(), "Tried to remove invalid button ID [" + std::to_string((int)id_) + "]!");
			buttonIDs.erase(std::remove(buttonIDs.begin(), buttonIDs.end(), id_));
		}

	private:
		StringID name;
		std::vector<ButtonID> buttonIDs;
	};

	struct ButtonAxis{
		constexpr ButtonAxis(ButtonID negative_, ButtonID positive_) : negative(negative_), positive(positive_){}

		ButtonID negative;
		ButtonID positive;
	};

	class Axis{
	public:
		Axis(StringID name_, float deadzone_ = 0.05f, bool invert_ = false) : name(name_), axisIDs(), buttonAxisIDs(), deadzone(deadzone_), invert(invert_){}

		Axis(StringID name_, AxisID axisID_, float deadzone_ = 0.05f, bool invert_ = false) : name(name_), axisIDs(), buttonAxisIDs(), deadzone(deadzone_), invert(invert_){
			axisIDs.insert(axisID_);
		}

		Axis(StringID name_, const ButtonAxis& buttonAxis_, float deadzone_ = 0.05f) : name(name_), axisIDs(), buttonAxisIDs(), deadzone(deadzone_), invert(false){
			buttonAxisIDs.push_back(buttonAxis_);
		}

		Axis(StringID name_, const std::set<AxisID>&& axisIDs_, float deadzone_ = 0.05f, bool invert_ = false) : name(name_), axisIDs(axisIDs_), buttonAxisIDs(), deadzone(deadzone_), invert(invert_){}
		Axis(StringID name_, const std::vector<ButtonAxis>&& buttonAxisIDs_, float deadzone_ = 0.05f) : name(name_), axisIDs(), buttonAxisIDs(buttonAxisIDs_), deadzone(deadzone_), invert(false){}

		StringID GetName() const{ return name; }
		const std::set<AxisID>& GetAxisIDs() const{ return axisIDs; }
		const std::vector<ButtonAxis>& GetButtonAxisIDs() const{ return buttonAxisIDs; }
		float GetDeadZone() const{ return deadzone; }
		bool IsInverted() const{ return invert; }

		void AddAxisID(AxisID id_){ axisIDs.insert(id_); }
		void AddButtonAxis(const ButtonAxis& buttonAxis_){ buttonAxisIDs.push_back(buttonAxis_); }

		void RemoveAxisID(AxisID id_){
			GADGET_ASSERT(axisIDs.find(id_) != axisIDs.end(), "Tried to remove invalid axis ID [" + std::to_string((int)id_) + "]!");
			axisIDs.erase(id_);
		}

	private:
		StringID name;
		std::set<AxisID> axisIDs;
		std::vector<ButtonAxis> buttonAxisIDs;

		bool invert;
		float deadzone;
	};
}

#endif //!GADGET_INPUT_TYPE_H
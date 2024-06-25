#ifndef GADGET_INPUT_TYPE_H
#define GADGET_INPUT_TYPE_H

#include <set>

#include "InputEnums.h"
#include "Debug.h"
#include "Math/Math.h"
#include "Utils/StringID.h"

namespace Gadget{
	class RawButton{
	public:
		constexpr RawButton(ButtonID buttonID_, bool isPressed_ = false) : buttonID(buttonID_), isPressed(isPressed_){
			GADGET_BASIC_ASSERT(buttonID_ < ButtonID::ButtonID_MAX);
		}

		constexpr ButtonID GetButtonID() const{ return buttonID; }
		constexpr bool IsPressed() const{ return isPressed; }

	private:
		const ButtonID buttonID;
		const bool isPressed;
	};

	class RawAxis{
	public:
		constexpr RawAxis(AxisID axisID_, float value_ = 0.0f) : axisID(axisID_), value(value_){
			GADGET_BASIC_ASSERT(axisID_ < AxisID::AxisID_MAX);
		}

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
		Button(StringID name_) : name(name_), buttonIDs(){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
		}

		Button(StringID name_, ButtonID buttonID_) : name(name_), buttonIDs(){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(buttonID_ < ButtonID::ButtonID_MAX);
			buttonIDs.insert(buttonID_);
		}

		Button(StringID name_, const std::set<ButtonID>&& buttonIDs_) : name(name_), buttonIDs(buttonIDs_){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(!buttonIDs_.empty());
			for(const auto & b : buttonIDs_){
				GADGET_BASIC_ASSERT(b < ButtonID::ButtonID_MAX);
			}
		}

		StringID GetName() const{ return name; }
		const std::set<ButtonID>& GetButtonIDs() const{ return buttonIDs; }

		void AddButtonID(ButtonID id_){
			GADGET_BASIC_ASSERT(id_ != ButtonID::ButtonID_MAX);
			buttonIDs.insert(id_);
		}

		void RemoveButtonID(ButtonID id_){
			GADGET_BASIC_ASSERT(id_ != ButtonID::ButtonID_MAX);
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
		MultiButton(StringID name_) : name(name_), buttonIDs(){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
		}

		MultiButton(StringID name_, ButtonID buttonID_) : name(name_), buttonIDs(){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(buttonID_ < ButtonID::ButtonID_MAX);
			buttonIDs.push_back(buttonID_);
		}

		MultiButton(StringID name_, const std::vector<ButtonID>&& buttonIDs_) : name(name_), buttonIDs(buttonIDs_){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(!buttonIDs_.empty());
			for(const auto& b : buttonIDs_){
				GADGET_BASIC_ASSERT(b < ButtonID::ButtonID_MAX);
			}
		}

		StringID GetName() const{ return name; }
		const std::vector<ButtonID>& GetButtonIDs() const{ return buttonIDs; }

		void AddButtonID(ButtonID id_){
			GADGET_BASIC_ASSERT(id_ != ButtonID::ButtonID_MAX);
			buttonIDs.push_back(id_);
		}

		void RemoveButtonID(ButtonID id_){
			GADGET_BASIC_ASSERT(id_ != ButtonID::ButtonID_MAX);
			GADGET_ASSERT(std::find(buttonIDs.begin(), buttonIDs.end(), id_) != buttonIDs.end(), "Tried to remove invalid button ID [" + std::to_string((int)id_) + "]!");
			buttonIDs.erase(std::remove(buttonIDs.begin(), buttonIDs.end(), id_));
		}

	private:
		StringID name;
		std::vector<ButtonID> buttonIDs;
	};

	struct ButtonAxis{
		constexpr ButtonAxis(ButtonID negative_, ButtonID positive_) : negative(negative_), positive(positive_){
			GADGET_BASIC_ASSERT(negative_ < ButtonID::ButtonID_MAX);
			GADGET_BASIC_ASSERT(positive_ < ButtonID::ButtonID_MAX);
		}

		ButtonID negative;
		ButtonID positive;
	};

	class Axis{
	public:
		Axis(StringID name_, float deadzone_ = 0.05f, bool invert_ = false) : name(name_), axisIDs(), buttonAxisIDs(), deadzone(deadzone_), invert(invert_){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(deadzone_));
			GADGET_BASIC_ASSERT(deadzone_ >= 0.0f);
			GADGET_BASIC_ASSERT(deadzone_ <= 1.0f);
		}

		Axis(StringID name_, AxisID axisID_, float deadzone_ = 0.05f, bool invert_ = false) : name(name_), axisIDs(), buttonAxisIDs(), deadzone(deadzone_), invert(invert_){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(axisID_ < AxisID::AxisID_MAX);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(deadzone_));
			GADGET_BASIC_ASSERT(deadzone_ >= 0.0f);
			GADGET_BASIC_ASSERT(deadzone_ <= 1.0f);

			axisIDs.insert(axisID_);
		}

		Axis(StringID name_, const ButtonAxis& buttonAxis_, float deadzone_ = 0.05f) : name(name_), axisIDs(), buttonAxisIDs(), deadzone(deadzone_), invert(false){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(buttonAxis_.negative < ButtonID::ButtonID_MAX);
			GADGET_BASIC_ASSERT(buttonAxis_.positive < ButtonID::ButtonID_MAX);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(deadzone_));
			GADGET_BASIC_ASSERT(deadzone_ >= 0.0f);
			GADGET_BASIC_ASSERT(deadzone_ <= 1.0f);

			buttonAxisIDs.push_back(buttonAxis_);
		}

		Axis(StringID name_, const std::set<AxisID>&& axisIDs_, float deadzone_ = 0.05f, bool invert_ = false) : name(name_), axisIDs(axisIDs_), buttonAxisIDs(), deadzone(deadzone_), invert(invert_){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(!axisIDs_.empty());
			GADGET_BASIC_ASSERT(Math::IsValidNumber(deadzone_));
			GADGET_BASIC_ASSERT(deadzone_ >= 0.0f);
			GADGET_BASIC_ASSERT(deadzone_ <= 1.0f);

			for(const auto& a : axisIDs_){
				GADGET_BASIC_ASSERT(a < AxisID::AxisID_MAX);
			}
		}

		Axis(StringID name_, const std::vector<ButtonAxis>&& buttonAxisIDs_, float deadzone_ = 0.05f) : name(name_), axisIDs(), buttonAxisIDs(buttonAxisIDs_), deadzone(deadzone_), invert(false){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(!buttonAxisIDs_.empty());
			GADGET_BASIC_ASSERT(Math::IsValidNumber(deadzone_));
			GADGET_BASIC_ASSERT(deadzone_ >= 0.0f);
			GADGET_BASIC_ASSERT(deadzone_ <= 1.0f);

			for(const auto& b : buttonAxisIDs_){
				GADGET_BASIC_ASSERT(b.negative < ButtonID::ButtonID_MAX);
				GADGET_BASIC_ASSERT(b.positive < ButtonID::ButtonID_MAX);
			}
		}

		StringID GetName() const{ return name; }
		const std::set<AxisID>& GetAxisIDs() const{ return axisIDs; }
		const std::vector<ButtonAxis>& GetButtonAxisIDs() const{ return buttonAxisIDs; }
		float GetDeadZone() const{ return deadzone; }
		bool IsInverted() const{ return invert; }

		void AddAxisID(AxisID id_){ axisIDs.insert(id_); }
		void AddButtonAxis(const ButtonAxis& buttonAxis_){ buttonAxisIDs.push_back(buttonAxis_); }
		void Invert(bool is_){ invert = is_; }

		void RemoveAxisID(AxisID id_){
			GADGET_BASIC_ASSERT(id_ < AxisID::AxisID_MAX);
			GADGET_ASSERT(axisIDs.find(id_) != axisIDs.end(), "Tried to remove invalid axis ID [" + std::to_string((int)id_) + "]!");
			axisIDs.erase(id_);
		}

	private:
		StringID name;
		std::set<AxisID> axisIDs;
		std::vector<ButtonAxis> buttonAxisIDs;

		float deadzone;
		bool invert;
	};
}

#endif //!GADGET_INPUT_TYPE_H
#ifndef GADGET_INPUT_H
#define GADGET_INPUT_H

#include <map>
#include <set>
#include <vector>

#include "InputGesture.h"
#include "InputType.h"
#include "Events/Event.h"

namespace Gadget{
	class Input{
	public:
		static Input* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		bool GetButtonDown(ButtonID id_) const;
		bool GetButtonDown(StringID buttonName_) const;

		bool GetButtonHeld(ButtonID id_) const;
		bool GetButtonHeld(StringID buttonName_) const;

		bool GetButtonUp(ButtonID id_) const;
		bool GetButtonUp(StringID buttonName_) const;

		float GetAxis(AxisID id_) const;
		float GetAxis(StringID axisName_) const;

		bool GetMultiButtonDown(StringID multiButton_) const;
		bool GetMultiButtonUp(StringID multiButton_) const;
		bool GetMultiButtonHeld(StringID multiButton_) const;

		void DefineButton(const Button&& button_);
		void DefineAxis(const Axis&& axis_);
		void DefineMultiButton(const MultiButton&& multiButton_);

		void ProcessInputs();

		static void OnEvent(const Event& e_);

	private:
		static Input* instance;

		Input();
		~Input();

		std::vector<RawButton> buttonEvents;
		std::vector<RawAxis> axisEvents;

		std::set<ButtonID> buttonsDown;
		std::set<ButtonID> buttonsHeld;
		std::set<ButtonID> buttonsUp;
		std::map<AxisID, float> axes;

		std::vector<Button> definedButtons;
		std::vector<Axis> definedAxes;
		std::vector<MultiButton> definedMultiButtons;

		//Delete unwanted compiler-generated copy/move constructors and assignment operators
		Input(const Input&) = delete;
		Input(Input&&) = delete;
		Input& operator=(const Input&) = delete;
		Input& operator=(Input&&) = delete;
	};
}

#endif // !GADGET_INPUT_H
#ifndef GADGET_INPUT_H
#define GADGET_INPUT_H

#include <map>
#include <set>
#include <vector>

#include "Data/StaticHashTable.h"
#include "Events/Event.h"
#include "Input/InputType.h"
#include "Utils/Utils.h"

namespace Gadget{
	class Input{
	public:
		#if defined GADGET_PLATFORM_SWITCH || defined GAGDET_PLATFORM_XBOX_SERIES
			static constexpr int MaxGamepads = 8;
		#else
			static constexpr int MaxGamepads = 4;
		#endif

		Input();
		~Input() = default;
		DISABLE_COPY_AND_MOVE(Input);

		bool GetButtonDown(ButtonID id_) const;
		bool GetButtonDown(StringID buttonName_) const;

		bool GetButtonHeld(ButtonID id_) const;
		bool GetButtonHeld(StringID buttonName_) const;

		bool GetButtonUp(ButtonID id_) const;
		bool GetButtonUp(StringID buttonName_) const;

		float GetAxis(AxisID id_) const;
		float GetAxis(StringID axisName_) const;

		int GetCurrentMouseXInPixels() const{ return currentMouseX; }
		int GetCurrentMouseYInPixels() const{ return currentMouseY; }
		float GetCurrentMouseXInGUICoordinates() const;
		float GetCurrentMouseYInGUICoordinates() const;

		bool GetMultiButtonDown(StringID multiButton_) const;
		bool GetMultiButtonUp(StringID multiButton_) const;
		bool GetMultiButtonHeld(StringID multiButton_) const;

		void DefineButton(const Button& button_);
		void DefineAxis(const Axis& axis_);
		void DefineMultiButton(const MultiButton& multiButton_);

		void ProcessInputs();

		void OnWindowRestartedEvent(const Event& e_);
		void OnKeyPressedEvent(const Event& e_);
		void OnKeyReleasedEvent(const Event& e_);
		void OnMouseMovedEvent(const Event& e_);
		void OnMouseScrollEvent(const Event& e_);
		void OnMouseButtonPressedEvent(const Event& e_);
		void OnMouseButtonReleasedEvent(const Event& e_);
		void OnGamepadAxisEvent(const Event& e_);
		void OnGamepadButtonPressedEvent(const Event& e_);
		void OnGamepadButtonReleasedEvent(const Event& e_);

	private:
		std::vector<RawButton> buttonEvents;
		std::vector<RawAxis> axisEvents;
		std::vector<RawAxis> persistentAxisEvents;

		std::set<ButtonID> buttonsDown;
		std::set<ButtonID> buttonsHeld;
		std::set<ButtonID> buttonsUp;
		StaticHashTable<AxisID, float, static_cast<int64_t>(AxisID::AxisID_MAX)> axes;
		StaticHashTable<AxisID, float, static_cast<int64_t>(AxisID::AxisID_MAX)> persistentAxes; //Special axes that are based on a persistent state rather than just motion (e.g. an analog stick)

		int currentMouseX;
		int currentMouseY;

		std::vector<Button> definedButtons;
		std::vector<Axis> definedAxes;
		std::vector<MultiButton> definedMultiButtons;
	};
}

#endif // !GADGET_INPUT_H
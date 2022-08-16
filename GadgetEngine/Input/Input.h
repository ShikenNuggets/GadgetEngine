#ifndef GADGET_INPUT_H
#define GADGET_INPUT_H

#include <map>
#include <set>
#include <vector>

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
		bool GetButtonHeld(ButtonID id_) const;
		bool GetButtonUp(ButtonID id_) const;
		float GetAxis(AxisID id_) const;

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

		//Delete unwanted compiler-generated copy/move constructors and assignment operators
		Input(const Input&) = delete;
		Input(Input&&) = delete;
		Input& operator=(const Input&) = delete;
		Input& operator=(Input&&) = delete;
	};
}

#endif // !GADGET_INPUT_H
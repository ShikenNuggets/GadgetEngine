#ifndef PONG_GAME_H
#define PONG_GAME_H

#include <App.h>
#include <GameInterface.h>
#include <Game/GameObject.h>
#include <Game/Scene.h>

#include "PongScene.h"

namespace Pong{
	class PongGame : public Gadget::GameInterface{
	public:
		PongGame() : GameInterface("Pong"){}

		virtual void LoadGame() override{
			Gadget::App::GetRenderer().SetClearColor(Gadget::Color::Black());
			DefineInputs();
			Gadget::App::GetSceneManager().AddScene(new PongScene());
		}

		void DefineInputs(){
			auto axis = Gadget::Axis(SID("LeftPaddleMove"));
			axis.AddAxisID(Gadget::AxisID::Gamepad_LeftStick_Vertical);
			axis.AddAxisID(Gadget::AxisID::Gamepad_DPad_Vertical);
			axis.AddButtonAxis(Gadget::ButtonAxis(Gadget::ButtonID::Keyboard_S, Gadget::ButtonID::Keyboard_W));
			Gadget::App::GetInput().DefineAxis(axis);

			axis = Gadget::Axis(SID("RightPaddleMove"));
			axis.AddAxisID(Gadget::AxisID::Gamepad_RightStick_Vertical);
			axis.AddButtonAxis(Gadget::ButtonAxis(Gadget::ButtonID::Keyboard_Arrow_Down, Gadget::ButtonID::Keyboard_Arrow_Up));
			Gadget::App::GetInput().DefineAxis(axis);
		}
	};
}

#endif //!PONG_GAME_H
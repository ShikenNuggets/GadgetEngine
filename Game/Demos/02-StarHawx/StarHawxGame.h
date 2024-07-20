#ifndef STAR_HAWX_GAME_H
#define STAR_HAWX_GAME_H

#include <App.h>
#include <GameInterface.h>
#include <Game/GameObject.h>
#include <Game/Scene.h>

#include "GameplayTestScene.h"

namespace StarHawx{
	class StarHawxGame : public Gadget::GameInterface{
	public:
		StarHawxGame() : GameInterface("StarHawx"){}

		virtual void LoadGame() final override{
			Gadget::App::GetRenderer().SetClearColor(Gadget::Color::Black());
			DefineInputs();
			DefineMaterials();
			Gadget::App::GetSceneManager().AddScene(new GameplayTestScene());
		}

		void DefineInputs(){
			auto axis = Gadget::Axis(UpDownMoveAxis);
			axis.AddAxisID(Gadget::AxisID::Gamepad_LeftStick_Vertical);
			axis.AddAxisID(Gadget::AxisID::Gamepad_DPad_Vertical);
			axis.AddButtonAxis(Gadget::ButtonAxis(Gadget::ButtonID::Keyboard_S, Gadget::ButtonID::Keyboard_W));
			Gadget::App::GetInput().DefineAxis(axis);

			axis = Gadget::Axis(LeftRightMoveAxis);
			axis.AddAxisID(Gadget::AxisID::Gamepad_LeftStick_Horizontal);
			axis.AddAxisID(Gadget::AxisID::Gamepad_DPad_Horizontal);
			axis.AddButtonAxis(Gadget::ButtonAxis(Gadget::ButtonID::Keyboard_A, Gadget::ButtonID::Keyboard_D));
			Gadget::App::GetInput().DefineAxis(axis);

			auto button = Gadget::Button(SID("MenuButton"));
			button.AddButtonID(Gadget::ButtonID::Gamepad_Start);
			button.AddButtonID(Gadget::ButtonID::Keyboard_Escape);
			Gadget::App::GetInput().DefineButton(button);

			button = Gadget::Button(SID("ShootButton"));
			button.AddButtonID(Gadget::ButtonID::Gamepad_Shoulder_Right2);
			button.AddButtonID(Gadget::ButtonID::Gamepad_Face_Down);
			button.AddButtonID(Gadget::ButtonID::Keyboard_Space);
		}

		void DefineMaterials(){
			Gadget::App::GetMaterialCache().AddMaterial(SID("BrickMaterial"), new Gadget::DiffuseTextureMaterial(SID("CubeTexture"), SID("DefaultShader")));
		}
	};
}

#endif //!STAR_HAWX_GAME_H
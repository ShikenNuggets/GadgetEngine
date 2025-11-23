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

			auto button = Gadget::Button(MenuButton);
			button.AddButtonID(Gadget::ButtonID::Gamepad_Start);
			button.AddButtonID(Gadget::ButtonID::Keyboard_Escape);
			Gadget::App::GetInput().DefineButton(button);

			button = Gadget::Button(ShootButton);
			button.AddButtonID(Gadget::ButtonID::Gamepad_Shoulder_Right2);
			button.AddButtonID(Gadget::ButtonID::Gamepad_Face_Down);
			button.AddButtonID(Gadget::ButtonID::Keyboard_Space);
			Gadget::App::GetInput().DefineButton(button);
		}

		void DefineMaterials(){
			Gadget::App::GetMaterialCache().AddMaterial(SID("BrickMaterial"), new Gadget::DiffuseTextureMaterial(SID("CubeTexture"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("RedMaterial"), new Gadget::ColorMaterial(Gadget::Color::Red(), SID("ColorShader")));

			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial1"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture1"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial2"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture2"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial3"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture3"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial4"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture4"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial5"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture5"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial6"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture6"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial7"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture7"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial8"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture8"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial9"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture9"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial10"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture10"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial11"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture11"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial12"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture12"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial13"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture13"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial14"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture14"), SID("UnlitShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("ArwingMaterial15"), new Gadget::DiffuseTextureMaterial(SID("ArwingTexture15"), SID("UnlitShader")));
		}
	};
}

#endif //!STAR_HAWX_GAME_H
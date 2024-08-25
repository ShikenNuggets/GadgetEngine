#ifndef EXAMPLE_GAME_H
#define EXAMPLE_GAME_H

#include <App.h>
#include <GameInterface.h>
#include <Game/GameObject.h>
#include <Game/Scene.h>

#include "ExampleScene.h"

namespace Example{
	class ExampleGame : public Gadget::GameInterface{
	public:
		ExampleGame() : GameInterface("Example Game"){}

		virtual void LoadGame() final override{
			DefineMaterials();
			Gadget::App::GetSceneManager().AddScene(new ExampleScene());
		}

		void DefineMaterials(){
			Gadget::App::GetMaterialCache().AddMaterial(SID("Invalid"), new Gadget::ColorMaterial(Gadget::Color::Pink(), SID("ColorShader")));

			Gadget::App::GetMaterialCache().AddMaterial(SID("BlueMaterial"), new Gadget::ColorMaterial(Gadget::Color::Blue(), SID("AnimColorShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("BlackMaterial"), new Gadget::ColorMaterial(Gadget::Color::Black(), SID("AnimColorShader")));
		}
	};
}

#endif //!EXAMPLE_GAME_H
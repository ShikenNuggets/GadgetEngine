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
			Gadget::Color lightBlue = Gadget::Color(0.14f, 0.44f, 0.68f);

			Gadget::App::GetMaterialCache().AddMaterial(SID("BlueMaterial"), new Gadget::ColorMaterial(lightBlue, SID("AnimColorShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("BlackMaterial"), new Gadget::ColorMaterial(Gadget::Color::Black(), SID("AnimColorShader")));
			Gadget::App::GetMaterialCache().AddMaterial(SID("BrickMaterial"), new Gadget::DiffuseTextureMaterial(SID("CubeTexture"), SID("DefaultShader")));

		}
	};
}

#endif //!EXAMPLE_GAME_H
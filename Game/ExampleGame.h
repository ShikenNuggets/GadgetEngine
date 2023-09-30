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
			Gadget::App::GetSceneManager().AddScene(new ExampleScene());
		}
	};
}

#endif //!EXAMPLE_GAME_H
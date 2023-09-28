#ifndef DEMO_REEL_GAME_H
#define DEMO_REEL_GAME_H

#include <App.h>
#include <GameInterface.h>
#include <Game/GameObject.h>
#include <Game/Scene.h>

#include "DemoReelScene.h"

namespace DemoReel{
	class DemoReelGame : public Gadget::GameInterface{
	public:
		DemoReelGame() : GameInterface("DemoReel"){}

		virtual void LoadGame() override{
			Gadget::App::GetRenderer().SetClearColor(Gadget::Color::Black());
			Gadget::App::GetSceneManager().AddScene(new DemoReelScene());
		}
	};
}

#endif //!DEMO_REEL_GAME_H
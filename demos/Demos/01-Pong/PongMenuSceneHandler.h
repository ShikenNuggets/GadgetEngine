#ifndef PONG_MENU_SCENE_HANDLER_H
#define PONG_MENU_SCENE_HANDLER_H

#include <App.h>
#include <Game/SceneComponent.h>
#include <Events/AppEvent.h>
#include <Events/EventHandler.h>

#include "PongState.h"

namespace Pong{
	class PongMenuSceneHandler : public Gadget::SceneComponent{
	public:
		PongMenuSceneHandler(Gadget::Scene* scene_) : SceneComponent(scene_){}

		virtual void OnUpdate([[maybe_unused]] float deltaTime_) final override{
			if(Gadget::App::GetInput().GetButtonDown(SID("QuitButton"))){
				Gadget::EventHandler::GetInstance()->HandleEvent(Gadget::WindowCloseEvent());
				return;
			}
		}

		void LoadOnePlayerGame(){
			PongState::currentMode = GameMode::SoloEasy;
			Gadget::App::GetSceneManager().RequestSceneLoad(1);
		}

		void LoadOnePlayerHardGame(){
			PongState::currentMode = GameMode::SoloHard;
			Gadget::App::GetSceneManager().RequestSceneLoad(1);
		}

		void LoadTwoPlayerGame(){
			PongState::currentMode = GameMode::TwoPlayer;
			Gadget::App::GetSceneManager().RequestSceneLoad(1);
		}

		void OpenSettings(){}
	};
}

#endif //!PONG_MENU_SCENE_HANDLER_H
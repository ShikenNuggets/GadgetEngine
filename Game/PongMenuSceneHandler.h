#ifndef PONG_MENU_SCENE_HANDLER_H
#define PONG_MENU_SCENE_HANDLER_H

#include <App.h>
#include <Game/SceneComponent.h>

namespace Pong{
	class PongMenuSceneHandler : public Gadget::SceneComponent{
	public:
		PongMenuSceneHandler(Gadget::Scene* scene_) : SceneComponent(scene_){}

		virtual void OnUpdate(float deltaTime_) override{}

		void LoadOnePlayerGame(){
			Gadget::App::GetSceneManager().RequestSceneLoad(1);
		}

		void LoadTwoPlayerGame(){}
		void OpenSettings(){}
	};
}

#endif //!PONG_MENU_SCENE_HANDLER_H
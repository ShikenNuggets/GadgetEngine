#ifndef PONG_SCENE_HANDLER_H
#define PONG_SCENE_HANDLER_H

#include <Graphics/GUI/GuiTextElement.h>

#include "Game/SceneComponent.h"

namespace Pong{
	class PongSceneHandler : public Gadget::SceneComponent{
	public:
		PongSceneHandler(Gadget::Scene* scene_);

		virtual void OnUpdate(float deltaTime_) override;

		void Reset();
		void AddScoreAndResetGame(int player_);

	private:
		int player1Score;
		int player2Score;
		Gadget::GuiTextElement* player1ScoreUI;
		Gadget::GuiTextElement* player2ScoreUI;
	};
}

#endif //!PONG_SCENE_HANDLER_H
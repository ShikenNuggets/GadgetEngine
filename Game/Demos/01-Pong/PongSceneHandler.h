#ifndef PONG_SCENE_HANDLER_H
#define PONG_SCENE_HANDLER_H

#include "Game/SceneComponent.h"

namespace Pong{
	class PongSceneHandler : public Gadget::SceneComponent{
	public:
		PongSceneHandler(Gadget::Scene* scene_);

		void Reset();
		void AddScoreAndResetGame(int player_);

	private:
		int player1Score;
		int player2Score;
	};
}

#endif //!PONG_SCENE_HANDLER_H
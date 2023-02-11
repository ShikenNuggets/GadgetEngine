#include "PongSceneHandler.h"

using namespace Pong;

#include "BallController.h"

PongSceneHandler::PongSceneHandler(Gadget::Scene* scene_) : SceneComponent(scene_), player1Score(0), player2Score(0){}

void PongSceneHandler::AddScoreAndResetGame(int player_){
	GADGET_BASIC_ASSERT(player_ == 1 || player_ == 2);
	if(player_ == 1){
		player1Score++;
	}else{
		player2Score++;
	}

	auto bc = parent->GetComponentInScene<BallController>();
	if(bc != nullptr){
		bc->GetParent()->SetPosition(Gadget::Vector3::Zero());
		bc->FlipVelocityX();
		bc->FlipVelocityY();
	}
}
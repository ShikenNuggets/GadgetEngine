#include "PongSceneHandler.h"

#include <App.h>

#include "BallController.h"
#include "PaddleController.h"

using namespace Pong;

PongSceneHandler::PongSceneHandler(Gadget::Scene* scene_) : SceneComponent(scene_), player1Score(0), player2Score(0){}

void PongSceneHandler::OnUpdate([[maybe_unused]] float deltaTime_){
	if(Gadget::App::GetInput().GetButtonDown(SID("ResetButton"))){
		Reset();
	}
}

void PongSceneHandler::Reset(){
	auto bc = parent->GetComponentInScene<BallController>();
	if(bc != nullptr){
		bc->Reset();
	}

	auto paddles = parent->GetAllComponentsInScene<PaddleController>();
	for(const auto& p : paddles){
		p->Reset();
	}
}

void PongSceneHandler::AddScoreAndResetGame(int player_){
	GADGET_BASIC_ASSERT(player_ == 1 || player_ == 2);
	if(player_ == 1){
		player1Score++;
	}else{
		player2Score++;
	}
}
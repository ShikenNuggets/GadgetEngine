#include "PongSceneHandler.h"

#include <App.h>
#include <Graphics/GUI/CanvasSceneComponent.h>

#include "BallController.h"
#include "PaddleController.h"

using namespace Pong;

PongSceneHandler::PongSceneHandler(Gadget::Scene* scene_) : SceneComponent(scene_), player1Score(0), player2Score(0), player1ScoreUI(nullptr), player2ScoreUI(nullptr){}

void PongSceneHandler::OnUpdate([[maybe_unused]] float deltaTime_){
	if(player1ScoreUI == nullptr || player2ScoreUI == nullptr){
		auto* canvas = parent->GetSceneComponent<Gadget::CanvasSceneComponent>();

		if(canvas != nullptr){
			player1ScoreUI = dynamic_cast<Gadget::GuiTextElement*>(canvas->GetCanvas().GetElement(SID("Score1")));
			player2ScoreUI = dynamic_cast<Gadget::GuiTextElement*>(canvas->GetCanvas().GetElement(SID("Score2")));
		}
	}

	if(Gadget::App::GetInput().GetButtonDown(SID("ResetButton"))){
		Reset();
	}

	if(Gadget::App::GetInput().GetButtonDown(SID("QuitButton"))){
		Gadget::App::GetSceneManager().RequestSceneLoad(0);
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

	if(player1ScoreUI != nullptr){
		player1ScoreUI->SetText(std::to_string(player1Score));
	}
	
	if(player2ScoreUI != nullptr){
		player2ScoreUI->SetText(std::to_string(player2Score));
	}

	Reset();
}
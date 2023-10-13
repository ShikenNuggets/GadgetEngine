#ifndef PONG_PADDLE_CONTROLLER_H
#define PONG_PADDLE_CONTROLLER_H

#include <App.h>
#include <Game/GameLogicComponent.h>

#include "PongState.h"

namespace Pong{
	class PaddleController : public Gadget::GameLogicComponent{
	public:
		PaddleController(Gadget::GameObject* parent_, unsigned int player_) : GameLogicComponent(parent_), paddleMoveSpeed(5.0f), player(player_), rigidbody(nullptr), ball(nullptr){
			GADGET_BASIC_ASSERT(player == 1 || player == 2);
		}

		virtual void OnStart() override{
			rigidbody = parent->GetComponent<Gadget::Rigidbody>();
			GADGET_BASIC_ASSERT(rigidbody != nullptr);

			startPosition = parent->GetPosition();

			ball = Gadget::App::GetInstance().GetSceneManager().CurrentScene()->FindWithTag(SID("Ball"));

			GameLogicComponent::OnStart();
		}

		float CalculateMoveAxis(){
			if(ball == nullptr){
				ball = Gadget::App::GetInstance().GetSceneManager().CurrentScene()->FindWithTag(SID("Ball"));
			}

			if(ball == nullptr){
				return 0.0f;
			}

			if(ball->GetPosition().y > parent->GetPosition().y){
				return 1.0f;
			}else if(ball->GetPosition().y < parent->GetPosition().y){
				return -1.0f;
			}

			return 0.0f;
		}

		virtual void OnUpdate(float deltaTime_) override{
			GADGET_BASIC_ASSERT(rigidbody != nullptr);

			float moveAxis = 0.0f;
			if(player == 1){
				moveAxis = Gadget::App::GetInput().GetAxis(SID("LeftPaddleMove"));
			}else if(player == 2 && PongState::currentMode == GameMode::TwoPlayer){
				moveAxis = Gadget::App::GetInput().GetAxis(SID("RightPaddleMove"));
			}else if(player == 2 && PongState::currentMode != GameMode::TwoPlayer){
				moveAxis = CalculateMoveAxis();
			}

			rigidbody->ClearForces();
			rigidbody->SetVelocity(Gadget::Vector3(0.0f, moveAxis * paddleMoveSpeed, 0.0f));

			GameLogicComponent::OnUpdate(deltaTime_);
		}

		void Reset(){
			parent->SetPosition(startPosition);
			rigidbody->Reset();
		}

	private:
		Gadget::Rigidbody* rigidbody;
		Gadget::GameObject* ball;

		const float paddleMoveSpeed;
		const unsigned int player;
		Gadget::Vector3 startPosition;
	};
}

#endif //!PONG_PADDLE_CONTROLLER_H
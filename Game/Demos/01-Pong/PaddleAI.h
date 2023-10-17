#ifndef PONG_PADDLE_AI_H
#define PONG_PADDLE_AI_H

#include <App.h>
#include <Game/GameLogicComponent.h>

#include "PaddleController.h"

namespace Pong{
	class PaddleAI : public PaddleController{
	public:
		PaddleAI(Gadget::GameObject* parent_, unsigned int player_) : PaddleController(parent_, player_), ball(nullptr){
			GADGET_BASIC_ASSERT(player == 1 || player == 2);
		}

		virtual void OnStart() override{
			ball = Gadget::GameObject::FindWithTag(SID("Ball"));
			PaddleController::OnStart();
		}

		//Calculate where the ball will be when it gets to this paddle
		float AdvancedCalculateTargetPosition(){
			Gadget::Rigidbody* ballRb = ball->GetComponent<Gadget::Rigidbody>();
			Gadget::Vector3 ballVelocity = ballRb->GetVelocity();

			//Ball is going towards the other guy
			if(ballVelocity.x < 0.0f){
				return 0.0f;
			}

			//Ball is coming towards us!
			if(ballVelocity.x > 0.0f){
				auto walls = Gadget::GameObject::FindObjectsWithTag(SID("Wall"));
				Gadget::StringID wallName = SID("BottomWall");
				bool useBottomWall = true;
				float wallPos = 0.0f;
				if(ballVelocity.y > 0.0f){
					wallName = SID("TopWall");
					useBottomWall = false;
				}

				for(const auto& wall : walls){
					GADGET_BASIC_ASSERT(wall != nullptr);
					if(wall == nullptr){
						Gadget::Debug::Log("Invalid GameObject* from FindObjectsWithTag call!", Gadget::Debug::Error, __FILE__, __LINE__);
						continue;
					}

					if(wall->GetName() == wallName){
						wallPos = wall->GetPosition().y;

						if(wallPos > 0.0f){
							wallPos -= wall->GetScale().y / 2.0f;
						} else{
							wallPos += wall->GetScale().y / 2.0f;
						}
					}
				}

				float distanceToWall = Gadget::Math::Abs(wallPos - (ball->GetPosition().y)) - (ball->GetScale().y / 2.0f);
				float distanceToPaddle = Gadget::Math::Abs((parent->GetPosition().x - (parent->GetScale().x / 2.0f)) - (ball->GetPosition().x)) - (ball->GetScale().x / 2.0f);

				float timeToBounceWall = distanceToWall / Gadget::Math::Abs(ballVelocity.y);
				float timeToHitPaddle = distanceToPaddle / Gadget::Math::Abs(ballVelocity.x);

				if(wallPos == 0.0f || timeToBounceWall > timeToHitPaddle){
					//Ball is coming towards us NOW! (or something went wrong with wallPos and we're just ignoring it)
					return ball->GetPosition().y + (ballVelocity.y * timeToHitPaddle);
				} else if(timeToHitPaddle > timeToBounceWall){
					//Ball is heading towards the wall - Figure out where it'll go after it bounces
					float ballPosXAfterBounce = ball->GetPosition().x + (ballVelocity.x * timeToBounceWall);
					float ballPosYAfterBounce = ball->GetPosition().y + (ballVelocity.y * timeToBounceWall);

					float newDistanceToPaddle = Gadget::Math::Abs((parent->GetPosition().x - (parent->GetScale().x / 2.0f)) - (ballPosXAfterBounce + ball->GetScale().x));
					float newTimeToHitPaddle = newDistanceToPaddle / ballVelocity.x;

					return ballPosYAfterBounce + ((-ballVelocity.y) * newTimeToHitPaddle);
				}
			}

			return 0.0f;
		}

		float CalculateMoveAxis(){
			if(ball == nullptr){
				ball = Gadget::GameObject::FindWithTag(SID("Ball"));
			}

			if(ball == nullptr){
				return 0.0f;
			}

			float targetPos = 0.0f;
			if(PongState::currentMode == GameMode::SoloEasy){
				targetPos = ball->GetPosition().y;
			} else{
				targetPos = AdvancedCalculateTargetPosition();
			}

			if(Gadget::Math::Abs(parent->GetPosition().y - targetPos) < 0.25f){
				return 0.0f;
			}

			//Simply move towards the ball
			if(targetPos > parent->GetPosition().y){
				return 1.0f;
			} else if(targetPos < parent->GetPosition().y){
				return -1.0f;
			}

			return 0.0f;
		}

		virtual void OnUpdate(float deltaTime_) override{
			GADGET_BASIC_ASSERT(rigidbody != nullptr);

			float moveAxis = CalculateMoveAxis();

			rigidbody->ClearForces();
			rigidbody->SetVelocity(Gadget::Vector3(0.0f, moveAxis * paddleMoveSpeed, 0.0f));

			GameLogicComponent::OnUpdate(deltaTime_); //Intentionally bypassing PaddleController::OnUpdate
		}

	private:
		Gadget::GameObject* ball;
	};
}

#endif //!PONG_PADDLE_AI_H
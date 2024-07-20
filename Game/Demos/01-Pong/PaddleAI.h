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

		PaddleAI(const Gadget::ComponentProperties& props_) : PaddleController(props_), ball(nullptr){
			Deserialize(props_);
		}

		virtual void OnStart() override{
			ball = Gadget::GameObject::FindWithTag(SID("Ball"));
			PaddleController::OnStart();
		}

		Gadget::Vector3 CalculateNextBounce(const Gadget::Vector3& startPos_, const Gadget::Vector3& ballScale_, const Gadget::Vector3& velocity_, float topWallPos_, float bottomWallPos_, float leftPaddlePos_, float rightPaddlePos_, int recursionCount_ = 0.0f) const{
			if(recursionCount_ > 10){
				Gadget::Debug::Log("Infinite recursion detected!", Gadget::Debug::Error, __FILE__, __LINE__);
				return Gadget::Vector3::Zero();
			}
			
			float wallPos = 0.0f;
			if(velocity_.y > 0.0f){
				wallPos = topWallPos_;
			}else if(velocity_.y < 0.0f){
				wallPos = bottomWallPos_;
			}else{
				return Gadget::Vector3::Zero();
			}

			float paddlePos = 0.0f;
			bool finalStage = false;
			if(velocity_.x > 0.0f){
				paddlePos = rightPaddlePos_;
				finalStage = true;
			}else{
				paddlePos = leftPaddlePos_;
			}

			float distanceToWall = Gadget::Math::Abs(wallPos - startPos_.y) - (ballScale_.y / 2.0f);
			float distanceToPaddle = Gadget::Math::Abs(paddlePos - startPos_.x) - (ballScale_.x / 2.0f);

			float timeToBounceWall = distanceToWall / Gadget::Math::Abs(velocity_.y);
			float timeToHitPaddle = distanceToPaddle / Gadget::Math::Abs(velocity_.x);

			if(wallPos == 0.0f || (timeToBounceWall > timeToHitPaddle && finalStage)){
				return startPos_ + (velocity_ * timeToHitPaddle);
			}else if(timeToBounceWall > timeToHitPaddle && !finalStage){
				float ballPosXAfterBounce = startPos_.x + (velocity_.x * timeToHitPaddle);
				float ballPosYAfterBounce = startPos_.x + (velocity_.x * timeToHitPaddle);

				return CalculateNextBounce(Gadget::Vector3(ballPosXAfterBounce, ballPosYAfterBounce, startPos_.z), ballScale_, Gadget::Vector3(velocity_.x * -1.5f, velocity_.y * 1.5f, velocity_.z), topWallPos_, bottomWallPos_, leftPaddlePos_, rightPaddlePos_, ++recursionCount_);
			}else{
				float ballPosXAfterBounce = startPos_.x + (velocity_.x * timeToBounceWall);
				float ballPosYAfterBounce = startPos_.y + (velocity_.y * timeToBounceWall);

				return CalculateNextBounce(Gadget::Vector3(ballPosXAfterBounce, ballPosYAfterBounce, startPos_.z), ballScale_, Gadget::Vector3(velocity_.x, -velocity_.y, velocity_.z), topWallPos_, bottomWallPos_, leftPaddlePos_, rightPaddlePos_, ++recursionCount_);
			}
		}

		float AdvancedCalculateTargetPosition(){
			Gadget::Rigidbody* ballRb = ball->GetComponent<Gadget::Rigidbody>();
			Gadget::Vector3 ballVelocity = ballRb->GetVelocity();

			
			float topWallPos = 0.0f;
			float bottomWallPos = 0.0f;

			auto walls = Gadget::GameObject::FindObjectsWithTag(SID("Wall"));
			for(const auto& wall : walls){
				GADGET_BASIC_ASSERT(wall != nullptr);
				if(wall == nullptr){
					Gadget::Debug::Log("Invalid GameObject* from FindObjectsWithTag call!", Gadget::Debug::Error, __FILE__, __LINE__);
					continue;
				}

				if(wall->GetName() == SID("TopWall")){
					topWallPos = wall->GetPosition().y - (wall->GetScale().y / 2.0f);
				}else if(wall->GetName() == SID("BottomWall")){
					bottomWallPos = wall->GetPosition().y + (wall->GetScale().y / 2.0f);
				}
			}

			float leftPaddlePos = 0.0f;
			float rightPaddlePos = 0.0f;
			auto paddles = Gadget::GameObject::FindObjectsWithTag(SID("Paddle"));
			for(const auto& paddle : paddles){
				GADGET_BASIC_ASSERT(paddle != nullptr);
				if(paddle == nullptr){
					Gadget::Debug::Log("Invalid GameObject* from FindObjectsWithTag call!", Gadget::Debug::Error, __FILE__, __LINE__);
					continue;
				}

				if(paddle->GetName() == SID("LeftPaddle")){
					leftPaddlePos = paddle->GetPosition().x + (paddle->GetScale().x / 2.0f);
				}else if(paddle->GetName() == SID("RightPaddle")){
					rightPaddlePos = paddle->GetPosition().x + (paddle->GetScale().x / 2.0f);
				}
			}

			return CalculateNextBounce(ball->GetPosition(), ball->GetScale(), ballVelocity, topWallPos, bottomWallPos, leftPaddlePos, rightPaddlePos).y;
		}

		float CalculateMoveAxis(float deltaTime_){
			if(ball == nullptr){
				ball = Gadget::GameObject::FindWithTag(SID("Ball"));
			}

			if(ball == nullptr){
				return 0.0f;
			}

			float targetPos = 0.0f;
			if(PongState::currentMode == GameMode::SoloEasy){
				targetPos = ball->GetPosition().y;
			}else{
				targetPos = AdvancedCalculateTargetPosition();
			}

			float multiplier = 1.0f;
			if(Gadget::Math::Abs(parent->GetPosition().y - targetPos) < (paddleMoveSpeed * deltaTime_)){
				multiplier = Gadget::Math::Abs(parent->GetPosition().y - targetPos) / (paddleMoveSpeed * deltaTime_);
			}

			//Simply move towards the ball
			if(targetPos > parent->GetPosition().y){
				return 1.0f * multiplier;
			}else if(targetPos < parent->GetPosition().y){
				return -1.0f * multiplier;
			}

			return 0.0f;
		}

		virtual void OnUpdate(float deltaTime_) override{
			GADGET_BASIC_ASSERT(rigidbody != nullptr);

			float moveAxis = CalculateMoveAxis(deltaTime_);

			rigidbody->ClearForces();
			rigidbody->SetVelocity(Gadget::Vector3(0.0f, moveAxis * paddleMoveSpeed, 0.0f));

			GameLogicComponent::OnUpdate(deltaTime_); //Intentionally bypassing PaddleController::OnUpdate
		}

		virtual Gadget::ComponentProperties Serialize() const override{ return PaddleController::Serialize(); }

	protected:
		virtual void Deserialize([[maybe_unused]] const Gadget::ComponentProperties& props_) override{}

	private:
		Gadget::GameObject* ball;
	};
}

#endif //!PONG_PADDLE_AI_H
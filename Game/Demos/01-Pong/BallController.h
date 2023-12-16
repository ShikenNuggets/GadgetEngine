#ifndef PONG_BALL_CONTROLLER_H
#define PONG_BALL_CONTROLLER_H

#include <App.h>
#include <Game/GameLogicComponent.h>
#include <Physics/Rigidbody.h>

#include "PongSceneHandler.h"

namespace Pong{
	class BallController : public Gadget::GameLogicComponent{
	public:
		BallController(Gadget::GameObject* parent_, float initialForce_, float playAreaWidth_, float playAreaHeight_) : GameLogicComponent(SID("BallController"), parent_), initialForce(initialForce_), currentForce(initialForce), rigidbody(nullptr), sceneHandler(nullptr), playAreaWidth(playAreaWidth_), playAreaHeight(playAreaHeight_), movingUp(true), movingRight(true), lastCollidedObject(0), roundOver(false){}

		virtual void OnStart() override{
			rigidbody = parent->GetComponent<Gadget::Rigidbody>();
			sceneHandler = Gadget::App::GetSceneManager().CurrentScene()->GetSceneComponent<PongSceneHandler>();
			GADGET_BASIC_ASSERT(rigidbody != nullptr);
			if(rigidbody != nullptr){
				rigidbody->SetVelocity(Gadget::Vector3(currentForce, currentForce / 2.0f, 0.0f));
			}

			playAreaWidth += 2.0f; //Add a small buffer to the play area
			playAreaHeight += 2.0f;

			GameLogicComponent::OnStart();
		}

		virtual void OnUpdate(float deltaTime_) override{
			Gadget::Rect playAreaRect = Gadget::Rect(0.0f, 0.0f, playAreaWidth, playAreaHeight);
			if(!playAreaRect.Intersects(parent->GetPosition().x, parent->GetPosition().y)){
				sceneHandler->Reset(); //Ball went out of bounds
			}

			GameLogicComponent::OnUpdate(deltaTime_);
		}

		virtual void OnCollision(const Gadget::Collision& col_) override{
			if(col_.otherName != lastCollidedObject){
				if(col_.HasTag(SID("Paddle"))){
					ApplyNewVelocity(true, false);
					CorrectPositionAfterPaddleCollision(col_);
				}else if(col_.HasTag(SID("Wall"))){
					ApplyNewVelocity(false, true, 0.0f);
					CorrectPositionAfterWallCollision(col_);
				}else if(col_.HasTag(SID("LeftGoal")) && !roundOver){
					roundOver = true;
					sceneHandler->AddScoreAndResetGame(2);
				}else if(col_.HasTag(SID("RightGoal")) && !roundOver){
					roundOver = true;
					sceneHandler->AddScoreAndResetGame(1);
				}
			}

			lastCollidedObject = col_.otherName;
			GameLogicComponent::OnCollision(col_);
		}

		void Reset(){
			while(HasCollisionsToHandle()){
				PopCollisionToHandle(); //Remove all pending collisions so they don't interfere with the new round
			}

			parent->SetPosition(Gadget::Vector3::Zero());
			parent->SetRotation(Gadget::Quaternion::Identity());

			rigidbody->Reset();

			currentForce = initialForce;
			ApplyNewVelocity(true, true, 0.0f);

			roundOver = false;
		}

	private:
		const float initialForce;
		float currentForce;
		Gadget::Rigidbody* rigidbody;
		PongSceneHandler* sceneHandler;
		float playAreaWidth;
		float playAreaHeight;

		bool movingUp;
		bool movingRight;

		Gadget::StringID lastCollidedObject;
		bool roundOver;

		void ApplyNewVelocity(bool flipX_, bool flipY_, float speedIncrease_ = 0.5f){
			currentForce += speedIncrease_;
			Gadget::Vector3 oldVelocity = rigidbody->GetVelocity();
			Gadget::Vector3 newVelocity = Gadget::Vector3::Zero();

			if(flipX_){
				movingRight = !movingRight;
			}

			if(flipY_){
				movingUp = !movingUp;
			}

			if(movingUp){
				newVelocity.y = currentForce / 2.0f;
			}else{
				newVelocity.y = -currentForce / 2.0f;
			}

			if(movingRight){
				newVelocity.x = currentForce;
			}else{
				newVelocity.x = -currentForce;
			}

			rigidbody->SetVelocity(newVelocity);
		}

		void CorrectPositionAfterPaddleCollision(const Gadget::Collision& col_){
			Gadget::Vector3 pos = GetParent()->GetPosition();
			float xScale = GetParent()->GetScale().x / 2.0f;
			float paddleX = 0.0f;
			if(col_.otherPos.x < 0.0f){
				paddleX = col_.otherPos.x + (col_.otherScale.x / 2.0f);
			}else{
				paddleX = col_.otherPos.x - (col_.otherScale.x / 2.0f);
				xScale = -xScale;
			}

			if((pos.x < 0.0f && pos.x < paddleX + xScale) || (pos.x > 0.0f && pos.x > paddleX + xScale)){
				GetParent()->SetPosition(paddleX + xScale, pos.y, pos.z);
			}
		}

		void CorrectPositionAfterWallCollision(const Gadget::Collision& col_){
			Gadget::Vector3 pos = GetParent()->GetPosition();
			float yScale = GetParent()->GetScale().y / 2.0f;
			float paddleY = 0.0f;
			if(col_.otherPos.y < 0.0f){
				paddleY = col_.otherPos.y + (col_.otherScale.y / 2.0f);
			}else{
				paddleY = col_.otherPos.y - (col_.otherScale.y / 2.0f);
				yScale = -yScale;
			}

			if((pos.y < 0.0f && pos.y < paddleY + yScale) || (pos.y > 0.0f && pos.y > paddleY + yScale)){
				GetParent()->SetPosition(pos.x, paddleY + yScale, pos.z);
			}
		}
	};
}

#endif //!PONG_BALL_CONTROLLER_H
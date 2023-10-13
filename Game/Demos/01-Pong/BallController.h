#ifndef PONG_BALL_CONTROLLER_H
#define PONG_BALL_CONTROLLER_H

#include <App.h>
#include <Game/GameLogicComponent.h>
#include <Physics/Rigidbody.h>

#include "PongSceneHandler.h"

namespace Pong{
	class BallController : public Gadget::GameLogicComponent{
	public:
		BallController(Gadget::GameObject* parent_, float initialForce_, float playAreaWidth_, float playAreaHeight_) : GameLogicComponent(parent_), initialForce(initialForce_), currentForce(initialForce), rigidbody(nullptr), sceneHandler(nullptr), playAreaWidth(playAreaWidth_), playAreaHeight(playAreaHeight_), movingUp(true), movingRight(true), collisionTimer(0.0f), ignorePaddleCollisions(false), ignoreWallCollisions(false), roundOver(false){}

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

			if(ignoreWallCollisions || ignorePaddleCollisions){
				collisionTimer += deltaTime_;
				if(collisionTimer > 0.1f){
					ignoreWallCollisions = false;
					ignorePaddleCollisions = false;
					collisionTimer = 0.0f;
				}
			}

			GameLogicComponent::OnUpdate(deltaTime_);
		}

		virtual void OnCollision(const Gadget::Collision& col_) override{
			if(col_.HasTag(SID("Paddle")) && !ignorePaddleCollisions){
				ApplyNewVelocity(true, false);
				CorrectPositionAfterCollision(col_);
				ignorePaddleCollisions = true; //Temporarily ignore paddle collisions to avoid wonky behaviour
			}else if(col_.HasTag(SID("Wall")) && !ignoreWallCollisions){
				ApplyNewVelocity(false, true, 0.0f);
				CorrectPositionAfterCollision(col_);
				ignoreWallCollisions = true; //Temporarily ignore wall collisions to avoid wonky behaviour
			}else if(col_.HasTag(SID("LeftGoal")) && !roundOver){
				roundOver = true;
				sceneHandler->AddScoreAndResetGame(2);
			}else if(col_.HasTag(SID("RightGoal")) && !roundOver){
				roundOver = true;
				sceneHandler->AddScoreAndResetGame(1);
			}

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

		float collisionTimer;
		bool ignorePaddleCollisions;
		bool ignoreWallCollisions;
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

		void CorrectPositionAfterCollision(const Gadget::Collision& col_){
			if(Gadget::Math::IsNearZero(col_.collisionVector.SquaredMagnitude()) || Gadget::Math::IsNearZero(col_.overlapAmount)){
				return;
			}

			Gadget::Vector3 collisionCorrection = -col_.collisionVector.Normalized() * col_.overlapAmount;

			if(col_.HasTag(SID("Paddle"))){
				parent->Translate(collisionCorrection.x, 0.0f, 0.0f);
			}else if(col_.HasTag(SID("Wall"))){
				parent->Translate(0.0f, collisionCorrection.y, 0.0f);
			}else{
				parent->Translate(collisionCorrection);
			}
		}
	};
}

#endif //!PONG_BALL_CONTROLLER_H
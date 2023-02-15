#ifndef PONG_BALL_CONTROLLER_H
#define PONG_BALL_CONTROLLER_H

#include <App.h>
#include <Game/GameLogicComponent.h>
#include <Physics/Rigidbody.h>

#include "PongSceneHandler.h"

namespace Pong{
	class BallController : public Gadget::GameLogicComponent{
	public:
		BallController(Gadget::GameObject* parent_, float initialForce_, float playAreaWidth_, float playAreaHeight_) : GameLogicComponent(parent_), initialForce(initialForce_), rigidbody(nullptr), sceneHandler(nullptr), playAreaWidth(playAreaWidth_), playAreaHeight(playAreaHeight_){}

		virtual void OnStart() override{
			rigidbody = parent->GetComponent<Gadget::Rigidbody>();
			sceneHandler = Gadget::App::GetSceneManager().CurrentScene()->GetSceneComponent<PongSceneHandler>();
			GADGET_BASIC_ASSERT(rigidbody != nullptr);
			if(rigidbody != nullptr){
				rigidbody->SetVelocity(Gadget::Vector3(initialForce, initialForce / 2.0f, 0.0f));
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
			if(col_.HasTag(SID("Paddle"))){
				FlipVelocityX();
				CorrectPositionAfterCollision(col_);
				IncreaseVelocity();
			}else if(col_.HasTag(SID("Wall"))){
				FlipVelocityY();
				CorrectPositionAfterCollision(col_);
			}else if(col_.HasTag(SID("LeftGoal"))){
				sceneHandler->AddScoreAndResetGame(1);
			}else if(col_.HasTag(SID("RightGoal"))){
				sceneHandler->AddScoreAndResetGame(2);
			}

			GameLogicComponent::OnCollision(col_);
		}

		void Reset(){
			parent->SetPosition(Gadget::Vector3::Zero());
			FlipVelocityX();
			FlipVelocityY();
			auto oldVelocity = rigidbody->GetVelocity();
			rigidbody->SetVelocity(Gadget::Vector3(
				Gadget::Math::Clamp(-initialForce, initialForce, oldVelocity.x), 
				Gadget::Math::Clamp(-initialForce / 2.0f, initialForce / 2.0f, oldVelocity.y),
				0.0f)
			);
		}

	private:
		const float initialForce;
		Gadget::Rigidbody* rigidbody;
		PongSceneHandler* sceneHandler;
		float playAreaWidth;
		float playAreaHeight;

		void FlipVelocityX(){ rigidbody->SetVelocity(Gadget::Vector3(-rigidbody->GetVelocity().x, rigidbody->GetVelocity().y, rigidbody->GetVelocity().z)); }
		void FlipVelocityY(){ rigidbody->SetVelocity(Gadget::Vector3(rigidbody->GetVelocity().x, -rigidbody->GetVelocity().y, rigidbody->GetVelocity().z)); }

		void IncreaseVelocity(){
			float xAmount = 0.1f;
			float yAmount = xAmount / 2.0f;

			if(rigidbody->GetVelocity().x < 0.0f){
				xAmount *= -1.0f;
			}

			if(rigidbody->GetVelocity().y < 0.0f){
				yAmount *= -1.0f;
			}

			rigidbody->AddVelocity(xAmount, yAmount, 0.0);
		}

		void CorrectPositionAfterCollision(const Gadget::Collision& col_){
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
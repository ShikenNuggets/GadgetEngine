#ifndef PONG_BALL_CONTROLLER_H
#define PONG_BALL_CONTROLLER_H

#include <App.h>
#include <Game/GameLogicComponent.h>
#include <Physics/Rigidbody.h>

#include "PongSceneHandler.h"

namespace Pong{
	class BallController : public Gadget::GameLogicComponent{
	public:
		BallController(Gadget::GameObject* parent_, float initialForce_) : GameLogicComponent(parent_), initialForce(initialForce_), rigidbody(nullptr), sceneHandler(nullptr){}

		virtual void OnStart() override{
			rigidbody = parent->GetComponent<Gadget::Rigidbody>();
			sceneHandler = Gadget::App::GetSceneManager().CurrentScene()->GetSceneComponent<PongSceneHandler>();
			GADGET_BASIC_ASSERT(rigidbody != nullptr);
			if(rigidbody != nullptr){
				rigidbody->SetVelocity(Gadget::Vector3(initialForce, initialForce / 2.0f, 0.0f));
			}

			GameLogicComponent::OnStart();
		}

		virtual void OnCollision(const Gadget::Collision& col_) override{
			if(col_.HasTag(SID("Paddle"))){
				FlipVelocityX();
				//IncreaseVelocity(); //Will re-enable this when position correcting is in, currently has inconsistent behaviour
			}else if(col_.HasTag(SID("Wall"))){
				FlipVelocityY();
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
	};
}

#endif //!PONG_BALL_CONTROLLER_H
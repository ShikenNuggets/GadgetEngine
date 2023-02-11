#ifndef PONG_BALL_CONTROLLER_H
#define PONG_BALL_CONTROLLER_H

#include <App.h>
#include <Game/GameLogicComponent.h>
#include <Physics/Rigidbody.h>

namespace Pong{
	class BallController : public Gadget::GameLogicComponent{
	public:
		BallController(Gadget::GameObject* parent_, float initialForce_) : GameLogicComponent(parent_), initialForce(initialForce_), rigidbody(nullptr){}

		virtual void OnStart() override{
			rigidbody = parent->GetComponent<Gadget::Rigidbody>();
			GADGET_BASIC_ASSERT(rigidbody != nullptr);
			if(rigidbody != nullptr){
				rigidbody->AddForce(Gadget::Vector3::Right() * initialForce);
				rigidbody->AddForce(Gadget::Vector3::Up() * initialForce / 2.0f);
			}

			GameLogicComponent::OnStart();
		}

		virtual void OnCollision(const Gadget::Collision& col_) override{
			if(col_.HasTag(SID("Paddle"))){
				rigidbody->SetVelocity(Gadget::Vector3(-rigidbody->GetVelocity().x, rigidbody->GetVelocity().y, rigidbody->GetVelocity().z));
			}else if(col_.HasTag(SID("Wall"))){
				rigidbody->SetVelocity(Gadget::Vector3(rigidbody->GetVelocity().x, -rigidbody->GetVelocity().y, rigidbody->GetVelocity().z));
			}else if(col_.HasTag(SID("Goal"))){

			}

			GameLogicComponent::OnCollision(col_);
		}

	private:
		const float initialForce;
		Gadget::Rigidbody* rigidbody;
	};
}

#endif //!PONG_BALL_CONTROLLER_H
#ifndef PONG_BALL_CONTROLLER_H
#define PONG_BALL_CONTROLLER_H

#include <App.h>
#include <Game/GameLogicComponent.h>
#include <Physics/Rigidbody.h>

namespace Pong{
	class BallController : public Gadget::GameLogicComponent{
	public:
		BallController(Gadget::GameObject* parent_, float initialForce_) : GameLogicComponent(parent_), initialForce(initialForce_){}

		virtual void OnStart() override{
			auto rb = parent->GetComponent<Gadget::Rigidbody>();
			GADGET_BASIC_ASSERT(rb != nullptr);
			if(rb != nullptr){
				rb->AddForce(Gadget::Vector3::Right() * initialForce);
				rb->AddForce(Gadget::Vector3::Up() * initialForce / 2.0f);
			}

			GameLogicComponent::OnStart();
		}

	private:
		const float initialForce;
	};
}

#endif //!PONG_BALL_CONTROLLER_H
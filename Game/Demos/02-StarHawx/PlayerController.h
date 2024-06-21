#ifndef STAR_HAWX_PLAYER_CONTROLLER_H
#define STAR_HAWX_PLAYER_CONTROLLER_H

#include <Game/GameLogicComponent.h>

#include "StarHawxDefines.h"

namespace StarHawx{
	class PlayerController : public Gadget::GameLogicComponent{
	public:
		PlayerController(Gadget::GameObject* parent_) : GameLogicComponent(SID("PlayerController"), parent_), rigidbody(nullptr), flightSpeed(5.0f){}

		virtual void OnStart() final override{
			GADGET_BASIC_ASSERT(parent != nullptr);
			rigidbody = parent->GetComponent<Gadget::Rigidbody>();
			GADGET_BASIC_ASSERT(rigidbody != nullptr);
		}

		virtual void OnUpdate(float deltaTime_) final override{
			GADGET_BASIC_ASSERT(rigidbody != nullptr);

			float upDownMove = Gadget::App::GetInput().GetAxis(StarHawx::UpDownMoveAxis) * flightSpeed;
			float leftRightMove = Gadget::App::GetInput().GetAxis(StarHawx::LeftRightMoveAxis) * flightSpeed;

			rigidbody->ClearForces();
			rigidbody->SetVelocity(Gadget::Vector3(leftRightMove, upDownMove, 0.0f));

			GameLogicComponent::OnUpdate(deltaTime_);
		}

	protected:
		Gadget::Rigidbody* rigidbody;

		float flightSpeed;
	};
}

#endif //!STAR_HAWX_PLAYER_CONTROLLER_H
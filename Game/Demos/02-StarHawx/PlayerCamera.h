#ifndef STAR_HAWX_PLAYER_CAMERA_H
#define STAR_HAWX_PLAYER_CAMERA_H

#include "PlayerController.h"
#include "StarHawxDefines.h"

namespace StarHawx{
	class PlayerCamera : public Gadget::GameLogicComponent{
	public:
		PlayerCamera(Gadget::GameObject* parent_) : GameLogicComponent(SID("PlayerCamera"), parent_), playerObj(nullptr), moveSpeed(5.0f), tiltSpeed(15.0f){}

		virtual void OnStart() final override{
			PlayerController* player = Gadget::App::GetSceneManager().CurrentScene()->GetComponentInScene<PlayerController>();
			GADGET_BASIC_ASSERT(player != nullptr);
			if(player != nullptr){
				playerObj = player->GetParent();
			}

			GameLogicComponent::OnStart();
		}

		virtual void OnUpdate(float deltaTime_) final override{
			GADGET_BASIC_ASSERT(parent != nullptr);
			GADGET_BASIC_ASSERT(playerObj != nullptr);

			const float distance = Gadget::Math::Abs(playerObj->GetPosition().x - parent->GetPosition().x);
			const float currentMoveSpeed = moveSpeed * (distance / 5.0f);
			const float rotateSpeed = tiltSpeed * (distance / 10.0f);
			if(playerObj->GetPosition().x > parent->GetPosition().x){
				parent->Translate(Gadget::Vector3::Lerp(Gadget::Vector3::Zero(), Gadget::Vector3(moveSpeed * deltaTime_, 0.0f, 0.0f), Gadget::Math::Clamp(0.0f, 1.0f, currentMoveSpeed / moveSpeed)));
				parent->SetRotation(Gadget::Euler::Lerp(noTilt, leftTilt, Gadget::Math::Clamp(0.0f, 1.0f, rotateSpeed / tiltSpeed)));
			}else if(playerObj->GetPosition().x < parent->GetPosition().x){
				parent->Translate(Gadget::Vector3::Lerp(Gadget::Vector3::Zero(), Gadget::Vector3(-moveSpeed * deltaTime_, 0.0f, 0.0f), Gadget::Math::Clamp(0.0f, 1.0f, currentMoveSpeed / moveSpeed)));
				parent->SetRotation(Gadget::Euler::Lerp(noTilt, rightTilt, Gadget::Math::Clamp(0.0f, 1.0f, rotateSpeed / tiltSpeed)));
			}else{
				parent->SetRotation(noTilt);
			}

			GameLogicComponent::OnUpdate(deltaTime_);
		}

	private:
		static constexpr Gadget::Euler noTilt = Gadget::Euler(0.0f, 0.0f, 0.0f);
		static constexpr Gadget::Euler leftTilt = Gadget::Euler(0.0f, 0.0f, -25.0f);
		static constexpr Gadget::Euler rightTilt = Gadget::Euler(0.0f, 0.0f, 25.0f);

		Gadget::GameObject* playerObj;
		float moveSpeed;
		float tiltSpeed;
	};
}

#endif //!STAR_HAWX_PLAYER_CAMERA_H
#ifndef STAR_HAWX_PLAYER_CAMERA_H
#define STAR_HAWX_PLAYER_CAMERA_H

#include "PlayerController.h"
#include "StarHawxDefines.h"

namespace StarHawx{
	class PlayerCamera : public Gadget::GameLogicComponent{
	public:
		PlayerCamera(Gadget::GameObject* parent_) : GameLogicComponent(SID("PlayerCamera"), parent_), playerObj(nullptr), moveSpeed(5.0f){}

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
			if(playerObj->GetPosition().x > parent->GetPosition().x){
				parent->Translate(Gadget::Vector3::Lerp(Gadget::Vector3::Zero(), Gadget::Vector3(moveSpeed * deltaTime_, 0.0f, 0.0f), Gadget::Math::Clamp(0.0f, 1.0f, currentMoveSpeed / moveSpeed)));
			}else if(playerObj->GetPosition().x < parent->GetPosition().x){
				parent->Translate(Gadget::Vector3::Lerp(Gadget::Vector3::Zero(), Gadget::Vector3(-moveSpeed * deltaTime_, 0.0f, 0.0f), Gadget::Math::Clamp(0.0f, 1.0f, currentMoveSpeed / moveSpeed)));
			}

			GameLogicComponent::OnUpdate(deltaTime_);
		}

	private:
		Gadget::GameObject* playerObj;
		float moveSpeed;
	};
}

#endif //!STAR_HAWX_PLAYER_CAMERA_H
#ifndef STARHAWX_PROJECTILE_CONTROLLER_H
#define STARHAWX_PROJECTILE_CONTROLLER_H

#include <Game/GameLogicComponent.h>

namespace StarHawx{
	class ProjectileController : public Gadget::GameLogicComponent{
	public:
		ProjectileController(Gadget::GameObject* parent_) : GameLogicComponent(SID("PlayerController"), parent_){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			Gadget::Rigidbody* rb = parent_->GetComponent<Gadget::Rigidbody>();
			GADGET_BASIC_ASSERT(rb != nullptr);
			if(rb){
				rb->AddForce(parent->GetTransform().Forward() * shotForce);
			}
		}

	private:
		static constexpr float shotForce = 50.0f;
	};
}

#endif //!STARHAWX_PROJECTILE_CONTROLLER_H
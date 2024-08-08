#ifndef STARHAWX_PROJECTILE_CONTROLLER_H
#define STARHAWX_PROJECTILE_CONTROLLER_H

#include <Game/GameLogicComponent.h>

namespace StarHawx{
	class ProjectileController : public Gadget::GameLogicComponent{
	public:
		ProjectileController(Gadget::GameObject* parent_) : GameLogicComponent(SID("PlayerController"), parent_), hasFired(false){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
		}

		virtual void OnUpdate(float deltaTime_) override{
			if(!hasFired){
				Gadget::Rigidbody* rb = parent->GetComponent<Gadget::Rigidbody>();
				GADGET_BASIC_ASSERT(rb != nullptr);
				if(rb){
					rb->SetVelocity(-parent->GetTransform().Forward() * shotForce);
				}

				hasFired = true;
			}
		}

	private:
		static constexpr float shotForce = 250.0f;
		bool hasFired;
	};
}

#endif //!STARHAWX_PROJECTILE_CONTROLLER_H
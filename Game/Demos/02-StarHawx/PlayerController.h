#ifndef STAR_HAWX_PLAYER_CONTROLLER_H
#define STAR_HAWX_PLAYER_CONTROLLER_H

#include <Game/GameLogicComponent.h>

namespace StarHawx{
	class PlayerController : public Gadget::GameLogicComponent{
	public:
		PlayerController(Gadget::GameObject* parent_) : GameLogicComponent(SID("PlayerController"), parent_){}

		virtual void OnStart() final override{}

		virtual void OnUpdate(float deltaTime_) final override{}
	};
}

#endif //!STAR_HAWX_PLAYER_CONTROLLER_H
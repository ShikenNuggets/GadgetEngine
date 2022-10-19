#ifndef GADGET_GAME_LOGIC_COMPONENT_H
#define GADGET_GAME_LOGIC_COMPONENT_H

#include "Component.h"

namespace Gadget{
	class GameLogicComponent : public Component{
	public:
		GameLogicComponent(GameObject* parent_) : Component(parent_){}
		virtual ~GameLogicComponent() override{}

		virtual void OnStart(){}
		virtual void OnUpdate([[maybe_unused]] float deltaTime_){}
		virtual void OnDestroy(){}
	};
}

#endif //!GADGET_GAME_LOGIC_COMPONENT_H

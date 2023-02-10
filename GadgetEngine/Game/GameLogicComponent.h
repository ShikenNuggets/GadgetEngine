#ifndef GADGET_GAME_LOGIC_COMPONENT_H
#define GADGET_GAME_LOGIC_COMPONENT_H

#include "Component.h"

namespace Gadget{
	class GameLogicComponent : public Component{
	public:
		GameLogicComponent(GameObject* parent_) : Component(parent_), hasStarted(false){}
		virtual ~GameLogicComponent() override{ OnDestroy(); }

		virtual void OnStart(){ hasStarted = true; }
		virtual void OnUpdate([[maybe_unused]] float deltaTime_){}
		virtual void OnDestroy(){}

		bool HasStarted() const{ return hasStarted; }

	private:
		bool hasStarted;
	};
}

#endif //!GADGET_GAME_LOGIC_COMPONENT_H

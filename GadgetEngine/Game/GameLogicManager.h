#ifndef GADGET_GAME_LOGIC_MANAGER_H
#define GADGET_GAME_LOGIC_MANAGER_H

#include "Game/Scene.h"

namespace Gadget{
	class GameLogicManager{
	public:
		GameLogicManager() = default;

		void Update(const Scene* scene_, float deltaTime_);
	};
}

#endif //!GADGET_GAME_LOGIC_MANAGER_H
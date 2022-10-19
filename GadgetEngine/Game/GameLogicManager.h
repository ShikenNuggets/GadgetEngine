#ifndef GADGET_GAME_LOGIC_MANAGER_H
#define GADGET_GAME_LOGIC_MANAGER_H

#include "Scene.h"

namespace Gadget{
	class GameLogicManager{
	public:
		GameLogicManager();

		void Update(const Scene* scene_, float deltaTime_);
	};
}

#endif //!GADGET_GAME_LOGIC_MANAGER_H
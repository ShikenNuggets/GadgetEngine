#ifndef GADGET_PHYS_MANAGER_H
#define GADGET_PHYS_MANAGER_H

#include "Game/Scene.h"

namespace Gadget{
	class PhysManager{
	public:
		PhysManager();
		~PhysManager();

		void Update(Scene* scene_, float deltaTime_);

	private:
		static PhysManager* instance;
	};
}

#endif //!GADGET_PHYS_MANAGER_H
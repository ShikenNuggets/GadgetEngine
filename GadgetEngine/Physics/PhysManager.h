#ifndef GADGET_PHYS_MANAGER_H
#define GADGET_PHYS_MANAGER_H

#include "Game/Scene.h"

namespace Gadget{
	class PhysManager{
	public:
		static PhysManager* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		void Update(Scene* scene_, float deltaTime_);

	private:
		static PhysManager* instance;

		PhysManager();
		~PhysManager();
	};
}

#endif //!GADGET_PHYS_MANAGER_H
#ifndef GADGET_PHYS_MANAGER_H
#define GADGET_PHYS_MANAGER_H

namespace Gadget{
	class PhysManager{
	public:
		static PhysManager* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		void Update(float deltaTime_);

	private:
		static PhysManager* instance;
		
		PhysManager();
		~PhysManager();
	};
}

#endif //!GADGET_PHYS_MANAGER_H
#ifndef GADGET_APP_H

#include "Memory/StackAllocator.h"

namespace Gadget{
	class App{
	public:
		static App* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		void Run();

		//Allocate memory that will free itself at the end of the frame
		//Do not cache this pointer beyond the frame boundary!!!
		void* AllocateSingleFrameMemory(size_t bytes_);

		//Allocate memory that will free itself at the end of the next frame
		//Only use this data for THIS frame or NEXT frame
		void* AllocateTwoFrameMemory(size_t bytes_);

	private:
		static App* instance;

		StackAllocator singleFrameAllocator;
		DoubleBufferedStackAllocator twoFrameAllocator;

		App();
		~App();

		void Initialize();
	};
}

#endif //!GADGET_APP_H
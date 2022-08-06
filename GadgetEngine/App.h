#ifndef GADGET_APP_H

#include <memory>

#include "Window.h"
#include "Events/Event.h"
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

		static void OnEvent(const Event& e_);

	private:
		static App* instance;

		bool isRunning;
		std::unique_ptr<Window> window;
		StackAllocator singleFrameAllocator;
		DoubleBufferedStackAllocator twoFrameAllocator;

		App();
		~App();

		void Initialize();

		//Delete unwanted compiler-generated copy/move constructors and assignment operators
		App(const App&) = delete;
		App(App&&) = delete;
		App& operator=(const App&) = delete;
		App& operator=(App&&) = delete;
	};
}

#endif //!GADGET_APP_H
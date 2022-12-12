#ifndef GADGET_APP_H
#define GADGET_APP_H

#include <memory>

#include "Config.h"
#include "GameInterface.h"
#include "Window.h"
#include "Core/Time.h"
#include "Events/Event.h"
#include "Game/BasicSceneManager.h"
#include "Game/GameLogicManager.h"
#include "Graphics/Renderer.h"
#include "Input/Input.h"
#include "Memory/StackAllocator.h"
#include "Resource/ResourceManager.h"

namespace Gadget{
	class App{
	public:
		static App* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		void Run(GameInterface& gameInterface_);

		//Allocate memory that will free itself at the end of the frame
		//Do not cache this pointer beyond the frame boundary!!!
		void* AllocateSingleFrameMemory(size_t bytes_);

		//Allocate memory that will free itself at the end of the next frame
		//Only use this data for THIS frame or NEXT frame
		void* AllocateTwoFrameMemory(size_t bytes_);

		static void OnEvent(const Event& e_);

		Renderer* GetRenderer() const{ return renderer.get(); }
		Renderer::API GetCurrentRenderAPI() const;
		float GetAspectRatio() const{ return renderer->GetAspectRatio(); }
		BasicSceneManager* GetSceneManager() const{ return sceneManager.get(); }

	private:
		static App* instance;

		bool isRunning;
		ResourceManager* resourceMgr;
		Config* config;
		Time* time;
		Input* input;
		std::unique_ptr<Renderer> renderer;
		std::unique_ptr<BasicSceneManager> sceneManager;
		std::unique_ptr<GameLogicManager> gameLogicManager;
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
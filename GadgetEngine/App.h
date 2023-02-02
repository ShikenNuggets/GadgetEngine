#ifndef GADGET_APP_H
#define GADGET_APP_H

#include <memory>

#include "Config.h"
#include "GameInterface.h"
#include "Core/Time.h"
#include "Game/BasicSceneManager.h"
#include "Game/GameLogicManager.h"
#include "Graphics/Renderer.h"
#include "Input/Input.h"
#include "Memory/StackAllocator.h"
#include "Physics/PhysManager.h"
#include "Resource/ResourceManager.h"

namespace Gadget{
	class App{
	public:
		App();
		~App();

		static App& GetInstance();
		static void DeleteInstance();

		void Run(GameInterface& gameInterface_);

		//Allocate memory that will free itself at the end of the frame
		//Do not cache this pointer beyond the frame boundary!!!
		void* AllocateSingleFrameMemory(size_t bytes_);

		//Allocate memory that will free itself at the end of the next frame
		//Only use this data for THIS frame or NEXT frame
		void* AllocateTwoFrameMemory(size_t bytes_);

		static void OnEvent(const Event& e_);

		std::string GetGameName() const{ return gameName; }
		Renderer::API GetCurrentRenderAPI() const;
		float GetAspectRatio() const{ return renderer->GetAspectRatio(); }

		ResourceManager& GetResourceManager() const{ return *resourceMgr; }
		Config& GetConfig() const{ return *config; }
		Time& GetTime() const{ return *time; }
		Input& GetInput() const{ return *input; }
		Renderer& GetRenderer() const{ return *renderer; }
		PhysManager& GetPhysics() const{ return *physics; }
		BasicSceneManager& GetSceneManager() const{ return *sceneManager; }
		GameLogicManager& GetGameLogicManager() const{ return *gameLogicManager; }

	private:
		static std::unique_ptr<App> instance;

		std::string gameName;
		bool isRunning;
		StackAllocator singleFrameAllocator;
		DoubleBufferedStackAllocator twoFrameAllocator;

		std::unique_ptr<ResourceManager> resourceMgr;
		std::unique_ptr<Config> config;
		std::unique_ptr<Time> time;
		std::unique_ptr<Input> input;
		std::unique_ptr<Renderer> renderer;
		std::unique_ptr<PhysManager> physics;
		std::unique_ptr<BasicSceneManager> sceneManager;
		std::unique_ptr<GameLogicManager> gameLogicManager;

		void Initialize(const std::string& name_);

		//Delete unwanted compiler-generated copy/move constructors and assignment operators
		App(const App&) = delete;
		App(App&&) = delete;
		App& operator=(const App&) = delete;
		App& operator=(App&&) = delete;
	};
}

#endif //!GADGET_APP_H
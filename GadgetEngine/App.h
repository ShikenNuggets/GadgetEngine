#ifndef GADGET_APP_H
#define GADGET_APP_H

#include <memory>

#include "Config.h"
#include "GameInterface.h"
#include "Core/Time.h"
#include "Game/BasicSceneManager.h"
#include "Game/GameLogicManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Materials/MaterialCache.h"
#include "Input/Input.h"
#include "Memory/GlobalAllocator.h"
#include "Memory/StackAllocator.h"
#include "Physics/PhysManager.h"
#include "Resource/ResourceManager.h"

namespace Gadget{
	//App is the core of the engine
	//All major engine subsystems are created and destroyed through here
	class App{
	public:
		App();
		~App();
		DISABLE_COPY_AND_MOVE(App);

		static App& GetInstance();
		static void DeleteInstance();

		void Run(GameInterface& gameInterface_);

		//Allocate memory that will free itself at the end of the frame
		//Do not cache this pointer beyond the frame boundary!!!
		void* AllocateSingleFrameMemory(size_t bytes_);

		//Allocate memory that will free itself at the end of the next frame
		//Only use this data for THIS frame or NEXT frame
		void* AllocateTwoFrameMemory(size_t bytes_);

		void OnWindowCloseEvent(const Event& e_);
		void OnWindowResizeEvent(const Event& e_);
		void OnMouseMoved(const Event& e_);
		void OnMouseButtonPressed(const Event& e_);

		static std::string GetGameName(){ return GetInstance().gameName; }
		static Renderer::API GetCurrentRenderAPI();
		static float GetAspectRatio(){ return GetInstance().renderer->GetAspectRatio(); }
		static float GetFixedDeltaTime();
		static float GetCurrentFramerateCap();

		static ResourceManager& GetResourceManager(){ return *GetInstance().resourceMgr; }
		static Config& GetConfig(){ return *GetInstance().config; }
		static Time& GetTime(){ return *GetInstance().time; }
		static Input& GetInput(){ return *GetInstance().input; }
		static MaterialCache& GetMaterialCache(){ return *GetInstance().materialCache; }
		static Renderer& GetRenderer(){ return *GetInstance().renderer; }
		static PhysManager& GetPhysics(){ return *GetInstance().physics; }
		static BasicSceneManager& GetSceneManager(){ return *GetInstance().sceneManager; }
		static GameLogicManager& GetGameLogicManager(){ return *GetInstance().gameLogicManager; }

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
		std::unique_ptr<MaterialCache> materialCache;
		std::unique_ptr<Renderer> renderer;
		std::unique_ptr<PhysManager> physics;
		std::unique_ptr<BasicSceneManager> sceneManager;
		std::unique_ptr<GameLogicManager> gameLogicManager;

		void Initialize(const std::string& name_);
		void Destroy();

		void InitRenderer();
		void ResetRenderer();

		inline bool IsFullyInitialized() const{
			return
				resourceMgr != nullptr
				&& config != nullptr
				&& time != nullptr
				&& input != nullptr
				&& renderer != nullptr
				&& physics != nullptr
				&& sceneManager != nullptr
				&& gameLogicManager != nullptr;
		}

		inline bool IsFullyDestroyed() const{
			return
				resourceMgr == nullptr
				&& config == nullptr
				&& time == nullptr
				&& input == nullptr
				&& renderer == nullptr
				&& physics == nullptr
				&& sceneManager == nullptr
				&& gameLogicManager == nullptr;
		}
	};
}

#endif //!GADGET_APP_H
#include "App.h"

#include <iostream>

#include "Config.h"
#include "Debug.h"
#include "Random.h"
#include "Core/Time.h"
#include "Game/ComponentFactory.h"
#include "Events/AppEvent.h"
#include "Events/EventHandler.h"
#include "Graphics/GUI/CanvasSceneComponent.h"
#include "Input/Input.h"
#include "Input/MouseEvent.h"
#include "Platform/Windows/Win32_GL_Renderer.h"
#include "Platform/Windows/Win32_DX12_Renderer.h"
#include "Platform/Windows/Win32_DXR_Renderer.h"
#include "Resource/ResourceManager.h"
#include "Utils/Profiler.h"

using namespace Gadget;

std::unique_ptr<App> App::instance = nullptr;

App::App() : gameName("GadgetEngine"), isRunning(true), singleFrameAllocator(1024), twoFrameAllocator(1024), resourceMgr(nullptr), config(nullptr), time(nullptr), input(nullptr), renderer(nullptr), physics(nullptr), sceneManager(nullptr), gameLogicManager(nullptr){
	GADGET_ASSERT(instance == nullptr, "Created multiple App instances!");
	
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowClose, std::bind(&App::OnWindowCloseEvent, this, std::placeholders::_1));
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowResize, std::bind(&App::OnWindowResizeEvent, this, std::placeholders::_1));
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseMoved, std::bind(&App::OnMouseMoved, this, std::placeholders::_1));
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseButtonPressed, std::bind(&App::OnMouseButtonPressed, this, std::placeholders::_1));
}

App::~App(){
	if(IsFullyInitialized()){
		Destroy();
	}

#ifdef GADGET_DEBUG
	EventHandler::DeleteInstance();
#endif //GADGET_DEBUG
}

App& App::GetInstance(){
	if(instance == nullptr){
		instance = std::make_unique<App>();
	}

	GADGET_ASSERT(instance != nullptr, "App instance was somehow nullptr! Nothing will work!");
	return *instance;
}

void App::DeleteInstance(){
	if(instance != nullptr){
		instance->Destroy(); //Destroy must be called before reset so that the App instance is still valid
		instance.reset();
	}
}

void App::Initialize(const std::string& name_){
	GADGET_BASIC_ASSERT(!name_.empty());
	GADGET_BASIC_ASSERT(IsFullyDestroyed());

	if(!name_.empty()){
		gameName = name_;
	}

	Debug::Init();
	resourceMgr = std::make_unique<ResourceManager>();
	config = std::make_unique<Config>();
	Random::SetSeed();
	time = std::make_unique<Time>();
	input = std::make_unique<Input>();

	#ifdef GADGET_RELEASE
	Debug::SetLogVerbosity(Debug::Warning);
	#endif // GADGET_RELEASE

	ComponentFactory::Init();

	GUID::SetInitialGUID(); //Temp, in the future we'll pull the number of unique entities from the project files or whatever, and then set that here

	materialCache = std::make_unique<MaterialCache>(); //Init before the renderer in case the renderer would like to cache some materials
	InitRenderer();

	physics = std::make_unique<PhysManager>();
	sceneManager = std::make_unique<BasicSceneManager>();
	gameLogicManager = std::make_unique<GameLogicManager>();

	GADGET_BASIC_ASSERT(IsFullyInitialized());
}

void App::Destroy(){
	GADGET_BASIC_ASSERT(IsFullyInitialized());

	gameLogicManager.reset();
	sceneManager.reset();
	physics.reset();
	renderer.reset();
	materialCache.reset();
	input.reset();
	time.reset();
	config.reset();
	resourceMgr.reset();

	GADGET_BASIC_ASSERT(IsFullyDestroyed());
}

void App::InitRenderer(){
	GADGET_BASIC_ASSERT(renderer == nullptr);
	if(renderer != nullptr){
		Debug::Log("Tried to re-initialize the renderer!", Debug::Warning, __FILE__, __LINE__);
		return;
	}

	int width = static_cast<int>(config->GetOptionFloat(EngineVars::Display::displayWidthKey));
	int height = static_cast<int>(config->GetOptionFloat(EngineVars::Display::displayHeightKey));
	int x = static_cast<int>(config->GetOptionFloat(EngineVars::Display::lastWindowXKey));
	int y = static_cast<int>(config->GetOptionFloat(EngineVars::Display::lastWindowYKey));

	//Use a default value if option is invalid
	if(width <= 100 || height <= 100){
		width = 1280;
		height = 720;
	}

#ifdef GADGET_PLATFORM_WIN32
	renderer = std::make_unique<Win32_GL_Renderer>(width, height, x, y);
	//renderer = std::make_unique<Win32_DX12_Renderer>(width, height, x, y);
	//renderer = std::make_unique<Win32_DXR_Renderer>(width, height, x, y);
#else
	static_assert(false, "Unhandled platform in App::Initialize!")
#endif //GADGET_PLATFORM_WIN32

	renderer->PostInit();
}

void App::ResetRenderer(){
	renderer.reset();
	InitRenderer();
}

void App::Run(GameInterface& gameInterface_){
	Initialize(gameInterface_.GetName()); //Init engine
	GADGET_BASIC_ASSERT(IsFullyInitialized());

	gameInterface_.LoadGame(); //Init game

	GADGET_BASIC_ASSERT(sceneManager->GetNumScenes() > 0);
	if(sceneManager->GetNumScenes() == 0){
		Debug::ThrowFatalError(SID("CORE"), "No scenes were added to the scene manager!", ErrorCode::Invalid_State, __FILE__, __LINE__);
	}

	sceneManager->LoadScene(0);

	time->Start();
	while(isRunning){
		//Main game loop
		Profiler::Start(SID("Main Loop"));

		//Clear single frame and two frame allocators
		//This must be the first thing that happens every frame
		singleFrameAllocator.Clear();

		twoFrameAllocator.SwapBuffers();
		twoFrameAllocator.CurrentBuffer().Clear();

		resourceMgr->DeleteAllUnusedResources(); //TODO - We don't necessarily need to do this every frame

		//Regular update follows
		time->Update();
		renderer->HandleWindowEvents();

		input->ProcessInputs();

		sceneManager->Update(time->DeltaTime());

		Profiler::Start(SID("Physics Update"));
		physics->Update(sceneManager->CurrentScene(), time->DeltaTime());
		Profiler::End(SID("Physics Update"));

		Profiler::Start(SID("Game Logic Update"));
		gameLogicManager->Update(sceneManager->CurrentScene(), time->DeltaTime());
		Profiler::End(SID("Game Logic Update"));

		Profiler::Start(SID("Render"));
		renderer->Render(sceneManager->CurrentScene());
		Profiler::End(SID("Render"));

		Profiler::End(SID("Main Loop"));

	#ifdef GADGET_DEBUG
		if(input->GetButtonDown(Gadget::ButtonID::Keyboard_F11)){
			ResetRenderer(); //TODO - Temp code, being able to hot reload the renderer is just useful for testing
			EventHandler::GetInstance()->HandleEvent(WindowRestartedEvent());
		}
	#endif // GADGET_DEBUG

		//After everything else is done, sleep for the appropriate amount of time (if necessary)
		time->Delay();
	}

	Profiler::OutputAllAverageResults();
}

void App::OnWindowCloseEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == WindowCloseEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	isRunning = false;
}

void App::OnWindowResizeEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == WindowResizedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const WindowResizedEvent* eventPtr = dynamic_cast<const WindowResizedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		renderer->OnResize(eventPtr->GetWidth(), eventPtr->GetHeight());
	}
}

void App::OnMouseMoved(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == MouseMovedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const MouseMovedEvent* eventPtr = dynamic_cast<const MouseMovedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		if(GetSceneManager().CurrentScene() != nullptr && GetSceneManager().CurrentScene()->GetSceneComponent<CanvasSceneComponent>() != nullptr){
			GetSceneManager().CurrentScene()->GetSceneComponent<CanvasSceneComponent>()->GetCanvas().OnMouseMoved(static_cast<int>(eventPtr->GetX()), static_cast<int>(eventPtr->GetY()));
		}
	}
}

void App::OnMouseButtonPressed(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == MouseButtonPressedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const MouseButtonPressedEvent* eventPtr = dynamic_cast<const MouseButtonPressedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		if(GetSceneManager().CurrentScene() != nullptr && GetSceneManager().CurrentScene()->GetSceneComponent<CanvasSceneComponent>() != nullptr){
			GetSceneManager().CurrentScene()->GetSceneComponent<CanvasSceneComponent>()->GetCanvas().OnMouseClick(eventPtr->GetButton());
		}
	}
}

void* App::AllocateSingleFrameMemory(size_t bytes_){
	GADGET_BASIC_ASSERT(IsFullyInitialized());

	if(bytes_ == 0){
		Debug::Log(SID("CORE"), "Tried to allocate 0 bytes!", Debug::Warning, __FILE__, __LINE__);
		return nullptr;
	}

	if(!singleFrameAllocator.CanAllocate(bytes_)){
		Debug::Log(SID("CORE"), "Single frame allocator is low on memory, cannot allocate " + std::to_string(bytes_) + " bytes!", Debug::Warning, __FILE__, __LINE__);
		return nullptr;
	}

	return singleFrameAllocator.Allocate(bytes_);
}

void* App::AllocateTwoFrameMemory(size_t bytes_){
	GADGET_BASIC_ASSERT(IsFullyInitialized());

	if(bytes_ == 0){
		Debug::Log(SID("CORE"), "Tried to allocate 0 bytes!", Debug::Warning, __FILE__, __LINE__);
		return nullptr;
	}

	if(!twoFrameAllocator.CurrentBuffer().CanAllocate(bytes_)){
		Debug::Log(SID("CORE"), "Two frame allocator is low on memory, cannot allocate " + std::to_string(bytes_) + " bytes!", Debug::Warning, __FILE__, __LINE__);
		return nullptr;
	}

	return twoFrameAllocator.CurrentBuffer().Allocate(bytes_);
}

Renderer::API App::GetCurrentRenderAPI(){
	GADGET_BASIC_ASSERT(instance != nullptr && instance->renderer != nullptr);

	if(GetInstance().renderer != nullptr){
		return GetInstance().renderer->GetRenderAPI();
	}else{
		Debug::Log(SID("CORE"), "GetCurrentRenderAPI called with no renderer loaded", Debug::Warning, __FILE__, __LINE__);
		return Renderer::API::None;
	}
}

float App::GetFixedDeltaTime(){
	GADGET_BASIC_ASSERT(instance != nullptr && instance->IsFullyInitialized());

	const float physicsUpdatesPerSecond = static_cast<float>(App::GetConfig().GetOptionFloat(EngineVars::Physics::physicsUpdatesKey));
	GADGET_BASIC_ASSERT(physicsUpdatesPerSecond > 0.0f);
	if(physicsUpdatesPerSecond <= 0.0f){
		Debug::Log("PhysicsUpdates was an invalid value!", Debug::Warning, __FILE__, __LINE__);
		return 0.0f; //Avoid divide by 0, or wonkiness caused by negative number
	}

	return 1.0f / physicsUpdatesPerSecond;
}

float App::GetCurrentFramerateCap(){
	GADGET_BASIC_ASSERT(instance != nullptr && instance->IsFullyInitialized());

	float targetFPS = static_cast<float>(GetConfig().GetOptionFloat(EngineVars::Display::targetFPSKey));
	bool vsync = GetConfig().GetOptionBool(EngineVars::Display::vsyncKey);

	if(targetFPS == 0.0f && vsync == false){
		return 0.0f;
	}

	float vsyncFPS = GetRenderer().GetRefreshRate();
	if(vsyncFPS != 0.0f && (targetFPS == 0.0f || vsyncFPS < targetFPS)){
		return vsyncFPS;
	}

	return targetFPS;
}
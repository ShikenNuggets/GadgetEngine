#include "App.h"

#include <iostream>

#include "Config.h"
#include "Debug.h"
#include "Random.h"
#include "Core/Time.h"
#include "Events/AppEvent.h"
#include "Events/EventHandler.h"
#include "Graphics/GUI/CanvasSceneComponent.h"
#include "Input/Input.h"
#include "Input/MouseEvent.h"
#include "Platform/Windows/Win32_GL_Renderer.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

std::unique_ptr<App> App::instance = nullptr;

App::App() : isRunning(true), gameName("GadgetEngine"), resourceMgr(nullptr), config(nullptr), time(nullptr), input(nullptr), physics(nullptr), renderer(nullptr), sceneManager(nullptr), gameLogicManager(nullptr), singleFrameAllocator(1024), twoFrameAllocator(1024){
	GADGET_ASSERT(instance == nullptr, "Created multiple App instances!");
	
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowClose, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowResize, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseMoved, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::MouseButtonPressed, OnEvent);
}

App::~App(){
	Destroy();
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
	gameName = name_;

	Debug::Init();
	resourceMgr = std::make_unique<ResourceManager>();
	config = std::make_unique<Config>();
	Random::SetSeed();
	time = std::make_unique<Time>();
	input = std::make_unique<Input>();

	#ifdef GADGET_RELEASE
	Debug::SetLogVerbosity(Debug::Warning);
	#endif // GADGET_RELEASE

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
	#else
		static_assert(false, "Unhandled platform in App::Initialize!")
	#endif //GADGET_PLATFORM_WIN32

	renderer->PostInit();

	physics = std::make_unique<PhysManager>();
	sceneManager = std::make_unique<BasicSceneManager>();
	gameLogicManager = std::make_unique<GameLogicManager>();
}

void App::Destroy(){
	gameLogicManager.reset();
	sceneManager.reset();
	physics.reset();
	renderer.reset();
	input.reset();
	time.reset();
	config.reset();
	resourceMgr.reset();
}

void App::Run(GameInterface& gameInterface_){
	Initialize(gameInterface_.GetName()); //Init engine
	gameInterface_.LoadGame(); //Init game
	sceneManager->LoadScene(0);

	time->Start();
	while(isRunning){
		//Main game loop

		//Clear single frame and two frame allocators
		//This must be the first thing that happens every frame
		singleFrameAllocator.Clear();

		twoFrameAllocator.SwapBuffers();
		twoFrameAllocator.CurrentBuffer().Clear();

		resourceMgr->DeleteAllUnusedResources(); //TODO - We don't necessarily need to do this every frame

		//Regular update follows
		time->Update();
		renderer->GetWindow().lock()->HandleEvents();

		input->ProcessInputs();

		sceneManager->Update(time->DeltaTime());

		physics->Update(sceneManager->CurrentScene(), time->DeltaTime());

		gameLogicManager->Update(sceneManager->CurrentScene(), time->DeltaTime());

		renderer->Render(sceneManager->CurrentScene());

		//After everything else is done, sleep for the appropriate amount of time (if necessary)
		time->Delay();
	}
}

void App::OnEvent(const Event& e_){
	if(e_.GetEventType() == EventType::WindowClose){
		GetInstance().isRunning = false;
	}else if(e_.GetEventType() == EventType::WindowResize){
		auto& ev = dynamic_cast<const WindowResizedEvent&>(e_); //TODO - dynamic cast is not particularly safe or efficient
		GetInstance().renderer->OnResize(ev.GetWidth(), ev.GetHeight());
	}else if(e_.GetEventType() == EventType::MouseMoved){
		auto& ev = dynamic_cast<const MouseMovedEvent&>(e_); //TODO - dynamic cast is not particularly safe or efficient

		if(GetInstance().GetSceneManager().CurrentScene() != nullptr && GetInstance().GetSceneManager().CurrentScene()->GetSceneComponent<CanvasSceneComponent>() != nullptr){
			GetInstance().GetSceneManager().CurrentScene()->GetSceneComponent<CanvasSceneComponent>()->GetCanvas().OnMouseMoved(ev.GetX(), ev.GetY());
		}
	}else if(e_.GetEventType() == EventType::MouseButtonPressed){
		auto& ec = dynamic_cast<const MouseButtonEvent&>(e_); //TODO - dynamic cast is not particularly safe or efficient

		if(GetInstance().GetSceneManager().CurrentScene() != nullptr && GetInstance().GetSceneManager().CurrentScene()->GetSceneComponent<CanvasSceneComponent>() != nullptr){
			GetInstance().GetSceneManager().CurrentScene()->GetSceneComponent<CanvasSceneComponent>()->GetCanvas().OnMouseClick(ec.GetButton());
		}
	}
}

void* App::AllocateSingleFrameMemory(size_t bytes_){
	if(bytes_ == 0 || !singleFrameAllocator.CanAllocate(bytes_)){
		return nullptr;
	}

	return singleFrameAllocator.Allocate(bytes_);
}

void* App::AllocateTwoFrameMemory(size_t bytes_){
	if(bytes_ == 0 || !twoFrameAllocator.CurrentBuffer().CanAllocate(bytes_)){
		return nullptr;
	}

	return twoFrameAllocator.CurrentBuffer().Allocate(bytes_);
}

Renderer::API App::GetCurrentRenderAPI(){
	if(GetInstance().renderer != nullptr){
		return GetInstance().renderer->GetRenderAPI();
	}else{
		return Renderer::API::None;
	}
}
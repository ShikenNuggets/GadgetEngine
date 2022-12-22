#include "App.h"

#include <iostream>

#include "Config.h"
#include "Debug.h"
#include "Random.h"
#include "Core/Time.h"
#include "Events/AppEvent.h"
#include "Events/EventHandler.h"
#include "Input/Input.h"
#include "Platform/Windows/Win32_Renderer.h"
#include "Resource/ResourceManager.h"

//Temp
#include "Graphics/Components/CameraComponent.h"
#include "Graphics/Components/LightComponent.h"
#include "Graphics/Components/RenderComponent.h"

using namespace Gadget;

App* App::instance = nullptr;

App::App() : isRunning(true), gameName("GadgetEngine"), resourceMgr(nullptr), config(nullptr), time(nullptr), input(nullptr), renderer(nullptr), sceneManager(nullptr), gameLogicManager(nullptr), singleFrameAllocator(1024), twoFrameAllocator(1024){
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowClose, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowResize, OnEvent);
}

App::~App(){
	gameLogicManager.reset(nullptr);
	sceneManager.reset(nullptr);
	renderer.reset(nullptr);
}

App* App::GetInstance(){
	if(instance == nullptr){
		instance = new App();
	}

	return instance;
}

#ifdef GADGET_DEBUG
void App::DeleteInstance(){
	if(instance != nullptr){
		Input::DeleteInstance();
		Time::DeleteInstance();
		Config::DeleteInstance();
		ResourceManager::DeleteInstance();

		delete instance;
		instance = nullptr;
	}
}
#endif //GADGET_DEBUG

void App::Initialize(const std::string& name_){
	gameName = name_;

	Debug::Init();

	resourceMgr = ResourceManager::GetInstance();
	GADGET_BASIC_ASSERT(resourceMgr);

	config = Config::GetInstance(); //Init Config
	GADGET_BASIC_ASSERT(config);

	Random::SetSeed();
	time = Time::GetInstance();
	GADGET_BASIC_ASSERT(time);

	input = Input::GetInstance();
	GADGET_BASIC_ASSERT(input);

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
	renderer = std::make_unique<Win32_Renderer>(width, height, x, y);
	#endif //GADGET_PLATFORM_WIN32

	renderer->PostInit();

	sceneManager = std::make_unique<BasicSceneManager>();

	gameLogicManager = std::make_unique<GameLogicManager>();
}

void App::Run(GameInterface& gameInterface_){
	Initialize(gameInterface_.GetName()); //Init engine
	gameInterface_.LoadGame(); //Init game
	sceneManager->LoadScene(0);

	Time::GetInstance()->Start();
	while(isRunning){
		//Main game loop

		//Clear single frame and two frame allocators
		//This must be the first thing that happens every frame
		singleFrameAllocator.Clear();

		twoFrameAllocator.SwapBuffers();
		twoFrameAllocator.CurrentBuffer().Clear();

		//Regular update follows
		Time::GetInstance()->Update();
		renderer->GetWindow().lock()->HandleEvents();

		Input::GetInstance()->ProcessInputs();

		gameLogicManager->Update(sceneManager->CurrentScene(), Time::GetInstance()->DeltaTime());

		renderer->Render(sceneManager->CurrentScene());

		//After everything else is done, sleep for the appropriate amount of time (if necessary)
		Time::GetInstance()->Delay();
	}
}

void App::OnEvent(const Event& e_){
	if(e_.GetEventType() == EventType::WindowClose){
		GetInstance()->isRunning = false;
	}else if(e_.GetEventType() == EventType::WindowResize){
		auto& ev = dynamic_cast<const WindowResizedEvent&>(e_); //TODO - dynamic cast is not particularly safe or efficient
		GetInstance()->renderer->OnResize(ev.GetWidth(), ev.GetHeight());
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

Renderer::API App::GetCurrentRenderAPI() const{
	if(renderer != nullptr){
		return renderer->GetRenderAPI();
	}else{
		return Renderer::API::None;
	}
}
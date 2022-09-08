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

using namespace Gadget;

App* App::instance = nullptr;

App::App() : isRunning(true), renderer(nullptr), singleFrameAllocator(1024), twoFrameAllocator(1024){
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowClose, OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowResize, OnEvent);
}

App::~App(){
	renderer.reset();
}

App* App::GetInstance(){
	if(instance == nullptr){
		instance = new App();
	}

	return instance;
}

#ifdef GADGET_DEBUG
void App::DeleteInstance(){
	instance->renderer.reset(nullptr);

	Input::DeleteInstance();
	Time::DeleteInstance();
	Config::DeleteInstance();
	ResourceManager::DeleteInstance();

	if(instance != nullptr){
		delete instance;
		instance = nullptr;
	}
}
#endif //GADGET_DEBUG

void App::Initialize(){
	ResourceManager::GetInstance();
	Config::GetInstance(); //Init Config
	Random::SetSeed();
	Time::GetInstance();
	Input::GetInstance();

	#ifdef GADGET_RELEASE
	Debug::SetLogVerbosity(Debug::Warning);
	#endif // GADGET_RELEASE

	#ifdef GADGET_PLATFORM_WIN32
	renderer = std::make_unique<Win32_Renderer>(1280, 720);
	#endif //GADGET_PLATFORM_WIN32
}

void App::Run(){
	Initialize();

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

		renderer->Render();

		//After everything else is done, sleep for the appropriate amount of time (if necessary)
		Time::GetInstance()->Delay();
	}
}

void App::OnEvent(const Event& e_){
	if(e_.GetEventType() == EventType::WindowClose){
		GetInstance()->isRunning = false;
	}else if(e_.GetEventType() == EventType::WindowResize){
		auto ev = dynamic_cast<const WindowResizedEvent&>(e_);
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
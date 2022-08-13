#include "App.h"

#include <iostream>

#include "Config.h"
#include "Debug.h"
#include "Random.h"
#include "Core/Time.h"
#include "Platform/Windows/Win32_Window.h"

#include "Events/EventHandler.h"

using namespace Gadget;

App* App::instance = nullptr;

App::App() : isRunning(true), window(nullptr), singleFrameAllocator(1024), twoFrameAllocator(1024){
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowClose, OnEvent);
}

App::~App(){
	window.reset();
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
		delete instance;
		instance = nullptr;
	}
}
#endif //GADGET_DEBUG

void App::Initialize(){
	Config::GetInstance(); //Init Config
	Random::SetSeed();
	Time::GetInstance();

	#ifdef GADGET_PLATFORM_WIN32
	window = std::make_unique<Win32_Window>(800, 600);
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
		window->HandleEvents();

		window->SwapBuffers();

		//After everything else is done, sleep for the appropriate amount of time (if necessary)
		Time::GetInstance()->Delay();
	}
}

void App::OnEvent(const Event& e_){
	if(e_.GetEventType() == EventType::WindowClose){
		GetInstance()->isRunning = false;
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
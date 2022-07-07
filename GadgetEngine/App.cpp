#include "App.h"

#include <iostream>

#include "Debug.h"
#include "Random.h"

using namespace Gadget;

App* App::instance = nullptr;

App* App::GetInstance(){
	if (instance == nullptr){
		instance = new App();
	}

	return instance;
}

#ifdef _DEBUG
void App::DeleteInstance(){
	if (instance != nullptr){
		delete instance;
		instance = nullptr;
	}
}
#endif //_DEBUG

void App::Initialize(){
	Random::SetSeed();
}

void App::Run(){
	Initialize();

	bool isRunning = true;
	while(isRunning){
		//Main game loop
		Debug::Log("Game engine is running, press enter to exit.", Debug::Verbose);
		std::cin.get();
		break;
	}
}
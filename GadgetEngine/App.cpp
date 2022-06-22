#include "App.h"

#include <iostream>

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
}

void App::Run(){
	Initialize();

	bool isRunning = true;
	while(isRunning){
		//Main game loop
		std::cout << "Game engine is running, press enter to exit." << std::endl;
		std::cin.get();
		break;
	}
}
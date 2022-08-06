#include "Win32_Window.h"

#include "Debug.h"
#include "Events/AppEvent.h"
#include "Events/EventHandler.h"

using namespace Gadget;

Win32_Window::Win32_Window(int w_, int h_) : Window(w_, h_){
	if(SDL_Init(SDL_INIT_EVERYTHING) > 0){
		Debug::Log("SDL could not be initialized! SDL Error: " + std::string(SDL_GetError()), Debug::LogType::FatalError, __FILE__, __LINE__);
		//TODO - Handle Fatal Error
	}

	Uint32 windowFlag = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	sdlWindow = SDL_CreateWindow("GadgetEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, windowFlag);
	if(sdlWindow == nullptr){
		Debug::Log("Window could not be created! SDL Error: " + std::string(SDL_GetError()), Debug::LogType::FatalError, __FILE__, __LINE__);
		//TODO - Handle Fatal Error
	}
}

Win32_Window::~Win32_Window(){
	if(sdlWindow != nullptr){
		SDL_DestroyWindow(sdlWindow);
		sdlWindow = nullptr;
	}

	//Close the SDL Subsystem
	SDL_Quit();
}

SDL_Window* Win32_Window::GetSDLWindow() const{ return sdlWindow; }

void Win32_Window::Update(){
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0){
		if(e.type == SDL_QUIT){
			EventHandler::GetInstance()->HandleEvent(WindowCloseEvent());
			return;
		}

		if(e.type == SDL_WINDOWEVENT){
			switch(e.window.event){
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					//TODO - Fire WindowResizedEvent
					break;
				default:
					break;
			}
		}
	}
}
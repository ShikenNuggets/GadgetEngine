#include "Win32_Window.h"

#include "Debug.h"
#include "Events/AppEvent.h"
#include "Events/EventHandler.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

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
		switch(e.type){
			case SDL_QUIT:
				EventHandler::GetInstance()->HandleEvent(WindowCloseEvent());
				return;
			case SDL_WINDOWEVENT:
				HandleWindowEvent(e);
				break;
			case SDL_KEYDOWN:
				EventHandler::GetInstance()->HandleEvent(KeyPressedEvent(e.key.keysym.sym)); //TODO - We probably shouldn't rely on SDL_Keycodes
				break;
			case SDL_KEYUP:
				EventHandler::GetInstance()->HandleEvent(KeyReleasedEvent(e.key.keysym.sym)); //TODO - We probably shouldn't rely on SDL_Keycodes
				break;
			case SDL_MOUSEMOTION:
				EventHandler::GetInstance()->HandleEvent(MouseMovedEvent(e.motion.xrel, e.motion.yrel)); //TODO - Not sure if relative motion is the ideal approach here
				break;
			case SDL_MOUSEBUTTONDOWN:
				EventHandler::GetInstance()->HandleEvent(MouseButtonPressedEvent(e.button.button));
				break;
			case SDL_MOUSEBUTTONUP:
				EventHandler::GetInstance()->HandleEvent(MouseButtonReleasedEvent(e.button.button));
				break;
			case SDL_MOUSEWHEEL:
				EventHandler::GetInstance()->HandleEvent(MouseScrollEvent(e.wheel.preciseX, e.wheel.preciseY));
				break;
			default:
				break;
		}

		if(e.type == SDL_QUIT){
			EventHandler::GetInstance()->HandleEvent(WindowCloseEvent());
			return;
		}else if(e.type == SDL_WINDOWEVENT){
			int w = 0, h = 0;

			switch(e.window.event){
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					SDL_GetWindowSize(sdlWindow, &w, &h);
					EventHandler::GetInstance()->HandleEvent(WindowResizedEvent(w, h));
					break;
				default:
					break;
			}
		}
	}
}

void Win32_Window::HandleWindowEvent(const SDL_Event& e_){
	int w = 0, h = 0;
	switch(e_.window.event){
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			SDL_GetWindowSize(sdlWindow, &w, &h);
			EventHandler::GetInstance()->HandleEvent(WindowResizedEvent(w, h));
			break;
		default:
			break;
	}
}
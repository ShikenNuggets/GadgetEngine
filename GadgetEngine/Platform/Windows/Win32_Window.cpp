#include "Win32_Window.h"

#include <glad/glad.h>

#include "Debug.h"
#include "Win32_Utils.h"
#include "Events/AppEvent.h"
#include "Events/EventHandler.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

using namespace Gadget;

Win32_Window::Win32_Window(int w_, int h_) : Window(w_, h_), sdlWindow(nullptr), glContext(nullptr){
	if(SDL_Init(SDL_INIT_EVERYTHING) > 0){
		Debug::Log("SDL could not be initialized! SDL Error: " + std::string(SDL_GetError()), Debug::LogType::FatalError, __FILE__, __LINE__);
		//TODO - Handle Fatal Error
	}

	//TODO - There's a lot of OpenGL specific code in here. Ideally the window and the rendering context are as separate as possible
	if(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) != 0){
		Debug::Log("Issue with setting OpenGL attribute! SDL Error: " + std::string(SDL_GetError()), Debug::LogType::FatalError, __FILE__, __LINE__);
		//TODO - Handle Fatal Error
	}

	if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) != 0){
		Debug::Log("Issue with setting OpenGL attribute! SDL Error: " + std::string(SDL_GetError()), Debug::LogType::FatalError, __FILE__, __LINE__);
		//TODO - Handle Fatal Error
	}

	if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6) != 0){
		Debug::Log("Issue with setting OpenGL attribute! SDL Error: " + std::string(SDL_GetError()), Debug::LogType::FatalError, __FILE__, __LINE__);
		//TODO - Handle Fatal Error
	}

	Uint32 windowFlag = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	sdlWindow = SDL_CreateWindow("GadgetEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, windowFlag);
	if(sdlWindow == nullptr){
		Debug::Log("Window could not be created! SDL Error: " + std::string(SDL_GetError()), Debug::LogType::FatalError, __FILE__, __LINE__);
		//TODO - Handle Fatal Error
	}

	glContext = SDL_GL_CreateContext(sdlWindow);
	if(glContext == nullptr){
		Debug::Log("OpenGL context could not be created! SDL Error: " + std::string(SDL_GetError()), Debug::LogType::FatalError, __FILE__, __LINE__);
		//TODO - Handle Fatal Error
	}

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		Debug::Log("Failed to initialize Glad! SDL Error: " + std::string(SDL_GetError()), Debug::LogType::FatalError, __FILE__, __LINE__);
		//TODO - Handle Fatal Error
	}

	glViewport(0, 0, GetWidth(), GetHeight());
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
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

void Win32_Window::HandleEvents(){
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
				EventHandler::GetInstance()->HandleEvent(KeyPressedEvent(Win32_Utils::ConvertSDLKeycodeToButtonID(e.key.keysym.sym)));
				break;
			case SDL_KEYUP:
				EventHandler::GetInstance()->HandleEvent(KeyReleasedEvent(Win32_Utils::ConvertSDLKeycodeToButtonID(e.key.keysym.sym)));
				break;
			case SDL_MOUSEMOTION:
				EventHandler::GetInstance()->HandleEvent(MouseMovedEvent(static_cast<float>(e.motion.xrel) / GetWidth(), static_cast<float>(e.motion.yrel) / GetHeight()));
				break;
			case SDL_MOUSEBUTTONDOWN:
				EventHandler::GetInstance()->HandleEvent(MouseButtonPressedEvent(Win32_Utils::ConvertSDLMouseButtonToButtonID(e.button.button)));
				break;
			case SDL_MOUSEBUTTONUP:
				EventHandler::GetInstance()->HandleEvent(MouseButtonReleasedEvent(Win32_Utils::ConvertSDLMouseButtonToButtonID(e.button.button)));
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

void Win32_Window::SwapBuffers(){
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(sdlWindow);
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
#ifndef GADGET_WIN32_WINDOW_H
#define GADGET_WIN32_WINDOW_H

#include <SDL.h>

#include "Window.h"

namespace Gadget{
	class Win32_Window : public Window{
	public:
		Win32_Window(int w_, int h_);
		virtual ~Win32_Window() override;

		SDL_Window* GetSDLWindow() const; //Avoid using the SDL_Window directly outside of this class - ideally create a new getter if you need something

		virtual void Update() override;

	private:
		SDL_Window* sdlWindow;

		void HandleWindowEvent(const SDL_Event& e_);
		void HandleKeyDownEvent(const SDL_Event& e_);
		void HandleKeyUpEvent(const SDL_Event& e_);
	};
}

#endif //!GADGET_WIN32_WINDOW_H
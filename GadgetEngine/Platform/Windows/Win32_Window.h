#ifndef GADGET_WIN32_WINDOW_H
#define GADGET_WIN32_WINDOW_H

#include <vector>

#pragma warning(disable : 26819) //Kill unfixable warning from SDL2
#include <SDL.h>
#pragma warning(default : 26819)

#include "Window.h"

namespace Gadget{
	class Win32_Window : public Window{
	public:
		Win32_Window(int w_, int h_);
		virtual ~Win32_Window() override;

		SDL_Window* GetSDLWindow() const; //Avoid using the SDL_Window directly outside of this class - ideally create a new getter if you need something

		virtual void HandleEvents() override;
		virtual void SwapBuffers() override;

	private:
		SDL_Window* sdlWindow;
		SDL_GLContext glContext;
		std::vector<SDL_Joystick*> joysticks;

		void HandleWindowEvent(const SDL_Event& e_);
		void HandleHatMotionEvent(const SDL_Event& e_);

		void RemoveJoystick(Sint32 instanceID_);
	};
}

#endif //!GADGET_WIN32_WINDOW_H
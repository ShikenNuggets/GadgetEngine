#ifndef GADGET_WIN32_WINDOW_H
#define GADGET_WIN32_WINDOW_H

#include <SDL.h>

#include "Window.h"

namespace Gadget{
	class Win32_Window : public Window{
	public:
		Win32_Window(int w_, int h_);
		virtual ~Win32_Window() override;

		SDL_Window* GetSDLWindow() const;

	private:
		SDL_Window* sdlWindow;
	};
}

#endif //!GADGET_WIN32_WINDOW_H
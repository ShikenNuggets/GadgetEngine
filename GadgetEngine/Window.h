#ifndef GADGET_WINDOW_H
#define GADGET_WINDOW_H

#include <string>

#include "ScreenCoordinate.h"

namespace Gadget{
	class Window{
	public:
		Window(int w_, int h_) : size(w_, h_){
			//Why the hell are you making a zero width / zero height window?
			_ASSERT(w_ > 0);
			_ASSERT(h_ > 0);
		}

		Window(const ScreenCoordinate& sc_) : size(sc_){
			//Why the hell are you making a zero width / zero height window?
			_ASSERT(w_ > 0);
			_ASSERT(h_ > 0);
		}

		virtual ~Window(){}

		inline int GetWidth() const{ return size.x; }
		inline int GetHeight() const{ return size.y; }
		inline ScreenCoordinate GetSize() const{ return size; }

		virtual void HandleEvents() = 0;
		virtual void SwapBuffers() = 0;

	protected:
		ScreenCoordinate size;
	};
}

#endif //!GADGET_WINDOW_H
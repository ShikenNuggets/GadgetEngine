#ifndef GADGET_WINDOW_H
#define GADGET_WINDOW_H

#include "Debug.h"
#include "ScreenCoordinate.h"

namespace Gadget{
	class Window{
	public:
		Window(int w_, int h_, int x_, int y_) : size(w_, h_), pos(x_, y_){
			GADGET_ASSERT(w_ > 0, "Tried to make a window with a width of 0!");
			GADGET_ASSERT(h_ > 0, "Tried to make a window with a height of 0!");
		}

		Window(const ScreenCoordinate& sc_, const ScreenCoordinate& pos_) : size(sc_), pos(pos_){
			GADGET_ASSERT(sc_.x > 0, "Tried to make a window with a width of 0!");
			GADGET_ASSERT(sc_.y > 0, "Tried to make a window with a height of 0!");
		}

		virtual ~Window(){}

		inline int GetWidth() const{ return size.x; }
		inline int GetHeight() const{ return size.y; }
		inline ScreenCoordinate GetSize() const{ return size; }

		virtual void HandleEvents() = 0;
		virtual void SwapBuffers() = 0;

	protected:
		ScreenCoordinate size;
		ScreenCoordinate pos;
	};
}

#endif //!GADGET_WINDOW_H
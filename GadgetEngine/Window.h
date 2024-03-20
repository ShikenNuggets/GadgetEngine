#ifndef GADGET_WINDOW_H
#define GADGET_WINDOW_H

#include "RenderSurface.h"

namespace Gadget{
	class Window : public RenderSurface{
	public:
		Window(int w_, int h_, int x_, int y_) : RenderSurface(w_, h_), pos(x_, y_){
			GADGET_ASSERT(w_ > 0, "Tried to make a window with a width of " + std::to_string(w_) + "!");
			GADGET_ASSERT(h_ > 0, "Tried to make a window with a height of " + std::to_string(h_) + "!");
		}

		Window(const ScreenCoordinate& sc_, const ScreenCoordinate& pos_) : RenderSurface(sc_), pos(pos_){
			GADGET_ASSERT(sc_.x > 0, "Tried to make a window with a width of " + std::to_string(sc_.x) + "!");
			GADGET_ASSERT(sc_.y > 0, "Tried to make a window with a height of " + std::to_string(sc_.y) + "!");
		}

		virtual ~Window(){}

	protected:
		ScreenCoordinate pos;
	};
}

#endif //!GADGET_WINDOW_H
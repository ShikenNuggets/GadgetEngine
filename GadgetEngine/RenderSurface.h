#ifndef GADGET_RENDER_SURFACE_H
#define GADGET_RENDER_SURFACE_H

#include "Debug.h"
#include "ScreenCoordinate.h"

namespace Gadget{
	class RenderSurface{
	public:
		RenderSurface(int w_, int h_) : size(w_, h_){
			GADGET_ASSERT(w_ > 0, "Tried to make a RenderSurface with a width of " + std::to_string(w_) + "!");
			GADGET_ASSERT(h_ > 0, "Tried to make a RenderSurface with a height of " + std::to_string(h_) + "!");
		}

		RenderSurface(const ScreenCoordinate& sc_) : size(sc_){
			GADGET_ASSERT(sc_.x > 0, "Tried to make a RenderSurface with a width of " + std::to_string(sc_.x) + "!");
			GADGET_ASSERT(sc_.y > 0, "Tried to make a RenderSurface with a height of " + std::to_string(sc_.y) + "!");
		}

		virtual ~RenderSurface(){}

		inline int GetWidth() const{ return size.x; }
		inline int GetHeight() const{ return size.y; }
		inline ScreenCoordinate GetSize() const{ return size; }

		virtual void HandleEvents() = 0; //TODO - Not sure if this should be here... we'll leave it here for now
		virtual void SwapBuffers() = 0;

		virtual float GetRefreshRate() = 0;

	protected:
		ScreenCoordinate size;
	};
}

#endif //!GADGET_RENDER_SURFACE_H
#ifndef GADGET_WINDOW_H
#define GADGET_WINDOW_H

#include "RenderSurface.h"

namespace Gadget{
	class Window{
	public:
		Window(int w_, int h_, int x_, int y_) : renderSurface(nullptr), size(w_, h_), pos(x_, y_){
			GADGET_ASSERT(w_ > 0, "Tried to make a window with a width of " + std::to_string(w_) + "!");
			GADGET_ASSERT(h_ > 0, "Tried to make a window with a height of " + std::to_string(h_) + "!");
		}

		Window(const ScreenCoordinate& sc_, const ScreenCoordinate& pos_) : renderSurface(nullptr), size(sc_.x, sc_.y), pos(pos_){
			GADGET_ASSERT(sc_.x > 0, "Tried to make a window with a width of " + std::to_string(sc_.x) + "!");
			GADGET_ASSERT(sc_.y > 0, "Tried to make a window with a height of " + std::to_string(sc_.y) + "!");
		}

		virtual ~Window(){
			delete renderSurface;
		}

		virtual uint64_t GetWindowHandle() const{ return 0; }
		inline RenderSurface* GetRenderSurface() const{ return renderSurface; }

		inline int GetWidth() const{
			if(renderSurface != nullptr){
				GADGET_BASIC_ASSERT(renderSurface->GetWidth() == size.x);
			}

			return size.x;
		}
		
		inline int GetHeight() const{
			if(renderSurface != nullptr){
				GADGET_BASIC_ASSERT(renderSurface->GetHeight() == size.y);
			}

			return size.y;
		}
		
		inline ScreenCoordinate GetSize() const{
			if(renderSurface != nullptr){
				GADGET_BASIC_ASSERT(renderSurface->GetWidth() == size.x);
				GADGET_BASIC_ASSERT(renderSurface->GetHeight() == size.y);
			}

			return size;
		}

		void SetRenderSurface(RenderSurface* surface_){
			renderSurface = surface_;
		}

		virtual void HandleEvents() = 0; //TODO - Not sure if this should be here... we'll leave it here for now
		virtual void SwapBuffers() = 0;

		virtual float GetRefreshRate() = 0;

	protected:
		RenderSurface* renderSurface;
		ScreenCoordinate size;
		ScreenCoordinate pos;
	};
}

#endif //!GADGET_WINDOW_H
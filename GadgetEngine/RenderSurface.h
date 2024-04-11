#ifndef GADGET_RENDER_SURFACE_H
#define GADGET_RENDER_SURFACE_H

#include "GadgetEnums.h"
#include "Debug.h"
#include "ScreenCoordinate.h"

namespace Gadget{
	class Window;

	class RenderSurface{
	public:
		RenderSurface(Window* parent_, int w_, int h_) : parent(parent_), size(w_, h_){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			GADGET_ASSERT(w_ > 0, "Tried to make a RenderSurface with a width of " + std::to_string(w_) + "!");
			GADGET_ASSERT(h_ > 0, "Tried to make a RenderSurface with a height of " + std::to_string(h_) + "!");
		}

		RenderSurface(Window* parent_, const ScreenCoordinate& sc_) : parent(parent_), size(sc_){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			GADGET_ASSERT(sc_.x > 0, "Tried to make a RenderSurface with a width of " + std::to_string(sc_.x) + "!");
			GADGET_ASSERT(sc_.y > 0, "Tried to make a RenderSurface with a height of " + std::to_string(sc_.y) + "!");
		}

		virtual ~RenderSurface(){}

		inline int GetWidth() const{ return size.x; }
		inline int GetHeight() const{ return size.y; }
		inline ScreenCoordinate GetSize() const{ return size; }

		inline virtual void SetWidth(int width_){ size.x = width_; }
		inline virtual void SetHeight(int height_){ size.y = height_; }

		inline virtual ErrorCode SetSize(const ScreenCoordinate& size_){
			size = size_;
			return ErrorCode::OK;
		}

	protected:
		Window* parent;
		ScreenCoordinate size;
	};
}

#endif //!GADGET_RENDER_SURFACE_H
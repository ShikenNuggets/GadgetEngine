#ifndef GADGET_VIEWPORT_RECT_H
#define GADGET_VIEWPORT_RECT_H

#include "Math/MathObjects.h"

namespace Gadget{
	namespace ViewportRect{
		//Note, these assume OpenGL non-device coordinates so may not work correctly with other graphics APIs
		static constexpr Rect Fullscreen	= Rect(0.0f, 0.0f, 1.0f, 1.0f);
		static constexpr Rect TopHalf		= Rect(0.0f, 0.5f, 1.0f, 0.5f);
		static constexpr Rect BottomHalf	= Rect(0.0f, 0.0f, 1.0f, 0.5f);
		static constexpr Rect LeftHalf		= Rect(0.0f, 0.0f, 0.5f, 1.0f);
		static constexpr Rect RightHalf		= Rect(0.5f, 0.0f, 0.5f, 1.0f);
		static constexpr Rect TopLeft		= Rect(0.0f, 0.5f, 0.5f, 0.5f);
		static constexpr Rect TopRight		= Rect(0.5f, 0.5f, 0.5f, 0.5f);
		static constexpr Rect BottomLeft	= Rect(0.0f, 0.0f, 0.5f, 0.5f);
		static constexpr Rect BottomRight	= Rect(0.5f, 0.0f, 0.5f, 0.5f);
	}
}

#endif //!GADGET_VIEWPORT_RECT_HN
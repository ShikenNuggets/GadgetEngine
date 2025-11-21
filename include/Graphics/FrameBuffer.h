#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "ScreenCoordinate.h"

namespace Gadget{
	class FrameBuffer{
	public:
		inline constexpr FrameBuffer(int width_, int height_) : size(width_, height_){}
		virtual ~FrameBuffer(){}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		inline constexpr ScreenCoordinate GetSize() const{ return size; }

	protected:
		ScreenCoordinate size;
	};
}

#endif //!FRAME_BUFFER_H
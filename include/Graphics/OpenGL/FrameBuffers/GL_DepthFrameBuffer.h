#ifndef GADGET_GL_DEPTH_FRAME_BUFFER_H
#define GADGET_GL_DEPTH_FRAME_BUFFER_H

#include "GL_FrameBuffer.h"

namespace Gadget{
	class GL_DepthFrameBuffer : public GL_FrameBuffer{
	public:
		GL_DepthFrameBuffer(int width_, int height_);
		virtual ~GL_DepthFrameBuffer() override;

		constexpr GLuint GetDepthMap() const{ return depthMap; }

	private:
		GLuint depthMap;
	};
}

#endif //!GADGET_GL_DEPTH_FRAME_BUFFER_H
#ifndef GL_DEFAULT_FRAME_BUFFER_H
#define GL_DEFAULT_FRAME_BUFFER_H

#include "GL_FrameBuffer.h"

namespace Gadget{
	class GL_DefaultFrameBuffer : public GL_FrameBuffer{
	public:
		GL_DefaultFrameBuffer(int width_, int height_);
		virtual ~GL_DefaultFrameBuffer() override;

		inline constexpr GLuint GetColorTexture() const{ return colorTexture; }
		inline constexpr GLuint GetDepthRBO() const{ return depthRBO; }

	private:
		GLuint colorTexture;
		GLuint depthRBO;
	};
}

#endif //!GL_DEFAULT_FRAME_BUFFER_H
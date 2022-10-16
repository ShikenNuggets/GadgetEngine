#ifndef GL_FRAME_BUFFER_H
#define GL_FRAME_BUFFER_H

#include <glad/glad.h>

#include "Graphics/FrameBuffer.h"

namespace Gadget{
	class GL_FrameBuffer : public FrameBuffer{
	public:
		GL_FrameBuffer(int width_, int height_);
		virtual ~GL_FrameBuffer() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		inline constexpr GLuint GetBuffer() const{ return buffer; }

	protected:
		GLuint buffer;
	};
}

#endif //!GL_FRAME_BUFFER_H
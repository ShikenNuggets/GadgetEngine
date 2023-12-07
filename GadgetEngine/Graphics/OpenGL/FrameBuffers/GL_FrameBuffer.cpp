#include "GL_FrameBuffer.h"

#include "Debug.h"

using namespace Gadget;

GL_FrameBuffer::GL_FrameBuffer(int width_, int height_) : FrameBuffer(width_, height_), buffer(0){
	GADGET_ASSERT(width_ > 0 && height_ > 0, "Framebuffer size must be greater than 0!");

	//TODO - I'd rather use the direct state access functions, but calling glTextureSubImage2D with framebuffers causes a crash later, unclear why
	glGenFramebuffers(1, &buffer);

	GADGET_BASIC_ASSERT(buffer != 0);
}

GL_FrameBuffer::~GL_FrameBuffer(){
	GADGET_BASIC_ASSERT(buffer != 0);
	glDeleteFramebuffers(1, &buffer);
}

void GL_FrameBuffer::Bind(){
	GADGET_BASIC_ASSERT(buffer != 0);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
}

void GL_FrameBuffer::Unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
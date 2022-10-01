#include "GL_FrameBuffer.h"

#include "Debug.h"

using namespace Gadget;

GL_FrameBuffer::GL_FrameBuffer(int width_, int height_) : FrameBuffer(width_, height_), buffer(0), colorTexture(0), depthRBO(0){
	GADGET_ASSERT(width_ > 0 && height_ > 0, "Framebuffer size must be greater than 0!");

	//TODO - I'd rather use the direct state access functions, but calling glTextureSubImage2D here causes a crash later, unclear why
	glGenFramebuffers(1, &buffer);
	Bind();
	
	//Color
	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

	//Depth and Stencil
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	GADGET_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "An error occured when creating the OpenGL Frame Buffer!");
	
	Unbind();
}

GL_FrameBuffer::~GL_FrameBuffer(){
	glDeleteFramebuffers(1, &buffer);
}

void GL_FrameBuffer::Bind(){
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
}

void GL_FrameBuffer::Unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
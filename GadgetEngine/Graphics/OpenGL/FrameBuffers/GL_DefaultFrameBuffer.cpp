#include "GL_DefaultFrameBuffer.h"

#include "Debug.h"

using namespace Gadget;

GL_DefaultFrameBuffer::GL_DefaultFrameBuffer(int width_, int height_) : GL_FrameBuffer(width_, height_), colorTexture(0), depthRBO(0){
	//TODO - I'd rather use the direct state access functions, but calling glTextureSubImage2D here causes a crash later, unclear why

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

GL_DefaultFrameBuffer::~GL_DefaultFrameBuffer(){
	glDeleteRenderbuffers(1, &depthRBO);
	glDeleteTextures(1, &colorTexture);
}
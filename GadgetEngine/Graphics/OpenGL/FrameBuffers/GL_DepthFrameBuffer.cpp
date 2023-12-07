#include "GL_DepthFrameBuffer.h"

#include "Debug.h"

using namespace Gadget;

GL_DepthFrameBuffer::GL_DepthFrameBuffer(int width_, int height_) : GL_FrameBuffer(width_, height_), depthMap(0){
	GADGET_BASIC_ASSERT(width_ > 0 && height_ > 0);
	//TODO - I'd rather use the direct state access functions, but calling glTextureSubImage2D here causes a crash later, unclear why

	Bind();

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width_, height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	Unbind();

	GADGET_BASIC_ASSERT(depthMap != 0);
}

GL_DepthFrameBuffer::~GL_DepthFrameBuffer(){
	GADGET_BASIC_ASSERT(depthMap != 0);
	glDeleteTextures(1, &depthMap);
}
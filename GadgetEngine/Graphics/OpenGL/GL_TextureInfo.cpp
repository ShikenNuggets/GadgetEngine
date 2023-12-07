#include "GL_TextureInfo.h"

#include "App.h"
#include "Debug.h"

using namespace Gadget;

GL_TextureInfo::GL_TextureInfo(const Texture& texture_) : TextureInfo(), textureID(0){
	GADGET_ASSERT(App::GetCurrentRenderAPI() == Renderer::API::OpenGL, "Tried to execute OpenGL commands on non-OpenGL render API!");
	GADGET_ASSERT(&texture_ == nullptr || texture_.GetWidth() > 0 && texture_.GetHeight() > 0 && !texture_.GetPixels().empty(), "Invalid texture data being passed to OpenGL!");

	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum internalFormat = GL_RGBA8;
	GLenum dataFormat = GL_BGRA;
	if(texture_.GetBitDepth() == 32){
		internalFormat = GL_RGBA8;
		dataFormat = GL_BGRA;
	}else if(texture_.GetBitDepth() == 24){
		Debug::Log(SID("RENDER"), "Using RGB image format, RGBA is preferred for optimal performance", Debug::Warning, __FILE__, __LINE__);
		internalFormat = GL_RGB8;
		dataFormat = GL_BGR;
	}else{
		Debug::Log("Invalid bit depth!", Debug::Error, __FILE__, __LINE__);
	}

	glTextureStorage2D(textureID, 1, internalFormat, texture_.GetWidth(), texture_.GetHeight());
	glTextureSubImage2D(textureID, 0, 0, 0, texture_.GetWidth(), texture_.GetHeight(), dataFormat, GL_UNSIGNED_BYTE, texture_.GetPixels().data());
	glGenerateTextureMipmap(textureID);

	GADGET_BASIC_ASSERT(textureID != 0);
}

GL_TextureInfo::~GL_TextureInfo(){
	GADGET_BASIC_ASSERT(textureID != 0);
	glDeleteTextures(1, &textureID);
}

void GL_TextureInfo::Bind(int textureIndex_){
	GADGET_BASIC_ASSERT(textureID != 0);
	GADGET_BASIC_ASSERT(textureIndex_ >= 0);

	glActiveTexture(GL_TEXTURE0 + textureIndex_); //TODO - Check the maximum and ensure we don't go over it
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void GL_TextureInfo::Unbind(){
	glBindTexture(GL_TEXTURE_2D, 0);
}
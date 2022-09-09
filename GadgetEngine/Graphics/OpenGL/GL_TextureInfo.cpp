#include "GL_TextureInfo.h"

using namespace Gadget;

GL_TextureInfo::GL_TextureInfo(const Texture& texture_) : TextureInfo(), textureID(0){
	_ASSERT(texture_.GetWidth() > 0 && texture_.GetHeight() > 0 && !texture_.GetPixels().empty()); //Invalid texture

	glGenTextures(1, &textureID);
	
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_.GetWidth(), texture_.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture_.GetPixels().data());
	glGenerateMipmap(GL_TEXTURE_2D);
	Unbind();
}

GL_TextureInfo::~GL_TextureInfo(){
	glDeleteTextures(1, &textureID);
}

void GL_TextureInfo::Bind(int textureIndex_){
	glActiveTexture(GL_TEXTURE0 + textureIndex_); //TODO - Check the maximum and ensure we don't go over it
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void GL_TextureInfo::Unbind(){
	glBindTexture(GL_TEXTURE_2D, 0);
}
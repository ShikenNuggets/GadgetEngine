#include "GL_TextureInfo.h"

#include "App.h"
#include "Debug.h"

using namespace Gadget;

GL_TextureInfo::GL_TextureInfo(const Texture& texture_) : TextureInfo(), textureID(0){
	GADGET_ASSERT(App::GetInstance()->GetCurrentRenderAPI() == Renderer::API::OpenGL, "Tried to execute OpenGL commands on non-OpenGL render API!");
	GADGET_ASSERT(&texture_ == nullptr || texture_.GetWidth() > 0 && texture_.GetHeight() > 0 && !texture_.GetPixels().empty(), "Invalid texture data being passed to OpenGL!");

	glGenTextures(1, &textureID);
	
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int color = GL_RGB;
	if(texture_.GetBitDepth() == 32){
		color = GL_RGBA;
	}else if(texture_.GetBitDepth() == 24){
		color = GL_RGB; //Redundant but good to be explicit
	}else{
		Debug::Log("Invalid bit depth!", Debug::Error, __FILE__, __LINE__);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, color, texture_.GetWidth(), texture_.GetHeight(), 0, color, GL_UNSIGNED_BYTE, texture_.GetPixels().data());
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
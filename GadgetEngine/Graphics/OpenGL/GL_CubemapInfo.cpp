#include "GL_CubemapInfo.h"

#include "Debug.h"

using namespace Gadget;

static constexpr int gBitDepthRGB = 24;
static constexpr int gBitDepthRGBA = 32;
static constexpr size_t gNumSides = 6;

GL_CubemapInfo::GL_CubemapInfo(const Cubemap& cubemap_) : textureID(0), vao(0), vbo(0), ebo(0){
	GADGET_BASIC_ASSERT(cubemap_.IsValid());

	//----------TEXTURE SETUP----------
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &textureID);

	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GADGET_BASIC_ASSERT(cubemap_.IsValid());

	GLenum internalFormat = GL_RGBA8;
	GLenum dataFormat = GL_RGBA;
	if(cubemap_.GetBitDepth() == gBitDepthRGBA){
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}else if(cubemap_.GetBitDepth() == gBitDepthRGB){
		Debug::Log(SID("RENDER"), "Using RGB image format, RGBA is preferred for optimal performance", Debug::Warning, __FILE__, __LINE__);
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}else{
		Debug::Log("Invalid bit depth!", Debug::Error, __FILE__, __LINE__);
	}

	glTextureStorage2D(textureID, 1, internalFormat, cubemap_.GetFaceWidth(), cubemap_.GetFaceHeight());

	for(size_t i = 0; i < gNumSides; i++){
		glTextureSubImage3D(textureID, 0, 0, 0, static_cast<GLint>(i), cubemap_.GetTexture(i)->GetWidth(), cubemap_.GetTexture(i)->GetHeight(), 1, dataFormat, GL_UNSIGNED_BYTE, cubemap_.GetTexture(i)->GetPixels().data());
	}
	glGenerateTextureMipmap(textureID);

	//----------VAO SETUP----------
	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &ebo);

	static constexpr std::array<GLfloat, 24> cubeVerts = {
		-1.0f,  1.0f, -1.0f, //0
		-1.0f, -1.0f, -1.0f, //1
		 1.0f, -1.0f, -1.0f, //2
		// 1.0f, -1.0f, -1.0f, //2
		 1.0f,  1.0f, -1.0f, //3
		//-1.0f,  1.0f, -1.0f, //0

		-1.0f, -1.0f,  1.0f, //4
		//-1.0f, -1.0f, -1.0f, //1
		//-1.0f,  1.0f, -1.0f, //0
		//-1.0f,  1.0f, -1.0f, //0
		-1.0f,  1.0f,  1.0f, //5
		//-1.0f, -1.0f,  1.0f, //4

		// 1.0f, -1.0f, -1.0f, //2
		 1.0f, -1.0f,  1.0f, //6
		 1.0f,  1.0f,  1.0f, //7
		// 1.0f,  1.0f,  1.0f, //7
		// 1.0f,  1.0f, -1.0f, //3
		// 1.0f, -1.0f, -1.0f, //2

		//-1.0f, -1.0f,  1.0f, //4
		//-1.0f,  1.0f,  1.0f, //5
		// 1.0f,  1.0f,  1.0f, //7
		// 1.0f,  1.0f,  1.0f, //7
		// 1.0f, -1.0f,  1.0f, //6
		//-1.0f, -1.0f,  1.0f, //4

		//-1.0f,  1.0f, -1.0f, //0
		// 1.0f,  1.0f, -1.0f, //3
		// 1.0f,  1.0f,  1.0f, //7
		// 1.0f,  1.0f,  1.0f, //7
		//-1.0f,  1.0f,  1.0f, //5
		//-1.0f,  1.0f, -1.0f, //0

		//-1.0f, -1.0f, -1.0f, //1
		//-1.0f, -1.0f,  1.0f, //4
		// 1.0f, -1.0f, -1.0f, //2
		// 1.0f, -1.0f, -1.0f, //2
		//-1.0f, -1.0f,  1.0f, //4
		// 1.0f, -1.0f,  1.0f //6
	};
	static constexpr std::array<GLuint, 36> cubeIndices = { 0, 1, 2, 2, 3, 0, 4, 1, 0, 0, 5, 4, 2, 6, 7, 7, 3, 2, 4, 5, 7, 7, 6, 4, 0, 3, 7, 7, 5, 0, 1, 4, 2, 2, 4, 6 };

	glNamedBufferData(vbo, sizeof(GLfloat) * cubeVerts.size(), cubeVerts.data(), GL_STATIC_DRAW);
	glNamedBufferData(ebo, sizeof(GLuint) * cubeIndices.size(), cubeIndices.data(), GL_STATIC_DRAW);

	//Position
	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GLfloat) * 3);
	glVertexArrayElementBuffer(vao, ebo);
}

GL_CubemapInfo::~GL_CubemapInfo(){
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteTextures(1, &textureID);
}

void GL_CubemapInfo::Bind(int textureIndex_){
	GADGET_BASIC_ASSERT(textureIndex_ >= 0);

	glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + textureIndex_)); //TODO - Check the maximum and ensure we don't go over it
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glBindVertexArray(vao);
}

void GL_CubemapInfo::Unbind(){
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);
}
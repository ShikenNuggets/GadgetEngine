#include "GL_ScreenQuad.h"

using namespace Gadget;

GL_ScreenQuad::GL_ScreenQuad() : ebo(0){
	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &ebo);

	static constexpr GLfloat triangleVerts[16] = {
		-1.0f, 1.0f,	0.0f, 1.0f, //0
		-1.0f, -1.0f,	0.0f, 0.0f, //1
		1.0f, -1.0f,	1.0f, 0.0f, //2

		//-1.0f, 1.0f,	0.0f, 1.0f, //0
		//1.0f, -1.0f,	1.0f, 0.0f, //2
		1.0f, 1.0f,		1.0f, 1.0f //3
	};
	static constexpr GLuint triangleIndices[6] = { 0, 1, 2, 0, 2, 3 };

	glNamedBufferData(vbo, sizeof(GLfloat) * 4 * 4, &triangleVerts[0], GL_STATIC_DRAW);
	glNamedBufferData(ebo, sizeof(GLuint) * 6 * 4, &triangleIndices[0], GL_STATIC_DRAW);

	//Position
	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);

	//Texture Coordinates
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2);

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GLfloat) * 4);
	glVertexArrayElementBuffer(vao, ebo);
}

GL_ScreenQuad::~GL_ScreenQuad(){
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void GL_ScreenQuad::Bind(){
	glBindVertexArray(vao);
}

void GL_ScreenQuad::Unbind(){
	glBindVertexArray(0);
}
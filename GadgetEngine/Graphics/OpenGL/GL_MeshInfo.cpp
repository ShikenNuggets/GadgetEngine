#include "GL_MeshInfo.h"

using namespace Gadget;

GL_MeshInfo::GL_MeshInfo() : MeshInfo(), vao(0), vbo(0){
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
}

GL_MeshInfo::~GL_MeshInfo(){
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void GL_MeshInfo::Bind(){
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void GL_MeshInfo::Unbind(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
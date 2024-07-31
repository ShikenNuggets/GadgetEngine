#include "GL_DynamicMeshInfo.h"

#include "App.h"
#include "Graphics/Vertex.h"

using namespace Gadget;

GL_DynamicMeshInfo::GL_DynamicMeshInfo([[maybe_unused]] size_t numVertices_, size_t numIndices_) : MeshInfo(numIndices_), vao(0), vbo(0){
	GADGET_ASSERT(App::GetCurrentRenderAPI() == Renderer::API::OpenGL, "Tried to execute OpenGL commands on non-OpenGL render API!");

	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GL_DynamicMeshInfo::~GL_DynamicMeshInfo(){
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void GL_DynamicMeshInfo::Bind(){
	glBindVertexArray(vao);
}

void GL_DynamicMeshInfo::Unbind(){
	glBindVertexArray(0);
}
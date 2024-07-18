#include "GL_MeshInfo.h"

#include "App.h"
#include "Graphics/Vertex.h"

using namespace Gadget;

GL_MeshInfo::GL_MeshInfo(const Submesh& mesh_) : MeshInfo(mesh_.indices.size()), vao(0), vbo(0), ebo(0){
	GADGET_ASSERT(App::GetCurrentRenderAPI() == Renderer::API::OpenGL, "Tried to execute OpenGL commands on non-OpenGL render API!");

	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &ebo);

	glNamedBufferData(vbo, mesh_.vertices.size() * sizeof(Vertex), &mesh_.vertices[0], GL_STATIC_DRAW);
	glNamedBufferData(ebo, mesh_.indices.size() * sizeof(uint32_t), &mesh_.indices[0], GL_STATIC_DRAW);

	//Position
	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

	//Normal
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3));

	//Texture Coordinates
	glEnableVertexArrayAttrib(vao, 2);
	glVertexArrayAttribBinding(vao, 2, 0);
	glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3) + sizeof(Vector3));

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(vao, ebo);

	GADGET_BASIC_ASSERT(vao != 0);
	GADGET_BASIC_ASSERT(vbo != 0);
	GADGET_BASIC_ASSERT(ebo != 0);
}

GL_MeshInfo::~GL_MeshInfo(){
	GADGET_BASIC_ASSERT(vao != 0);
	GADGET_BASIC_ASSERT(vbo != 0);
	GADGET_BASIC_ASSERT(ebo != 0);

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void GL_MeshInfo::Bind(){
	GADGET_BASIC_ASSERT(vao != 0);

	glBindVertexArray(vao);
}

void GL_MeshInfo::Unbind(){
	glBindVertexArray(0);
}
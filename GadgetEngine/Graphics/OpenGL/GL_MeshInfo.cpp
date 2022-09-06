#include "GL_MeshInfo.h"

#include "Graphics/Vertex.h"

using namespace Gadget;

GL_MeshInfo::GL_MeshInfo(const Mesh& mesh_) : MeshInfo(), vao(0), vbo(0), ebo(0){
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	Bind();

	static constexpr GLfloat triangleVerts[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};
	static constexpr GLuint triangleIndices[] = { 0, 1, 2 };

	glBufferData(GL_ARRAY_BUFFER, mesh_.vertices.size() * sizeof(Vertex), &mesh_.vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_.indices.size() * sizeof(uint32_t), &mesh_.indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((GLvoid*)(sizeof(Vector3) + sizeof(Vector3))));
	glEnableVertexAttribArray(2);
	Unbind();
}

GL_MeshInfo::~GL_MeshInfo(){
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void GL_MeshInfo::Bind(){
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void GL_MeshInfo::Unbind(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
#ifndef GADGET_GL_DYNAMIC_MESH_INFO_H
#define GADGET_GL_DYNAMIC_MESH_INFO_H

#include <glad/glad.h>

#include "Graphics/MeshInfo.h"

namespace Gadget{
	class GL_DynamicMeshInfo : public MeshInfo{
	public:
		GL_DynamicMeshInfo(size_t numVertices_, size_t numIndices_);
		virtual ~GL_DynamicMeshInfo() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		GLuint GetVAO() const{ return vao; }
		GLuint GetVBO() const{ return vbo; }

	private:
		GLuint vao; //Vertex array object
		GLuint vbo; //Vertex buffer object
	};
}

#endif //!GADGET_GL_DYNAMIC_MESH_INFO_H
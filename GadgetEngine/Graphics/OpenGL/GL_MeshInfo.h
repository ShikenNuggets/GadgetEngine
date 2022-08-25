#ifndef GADGET_GL_MESH_INFO_H
#define GADGET_GL_MESH_INFO_H

#include <glad/glad.h>

#include "Graphics/MeshInfo.h"

namespace Gadget{
	class GL_MeshInfo : public MeshInfo{
	public:
		GL_MeshInfo();
		virtual ~GL_MeshInfo() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		GLuint GetVAO() const{ return vao; }
		GLuint GetVBO() const{ return vbo; }
		GLuint GetEBO() const{ return ebo; }

	private:
		GLuint vao; //Vertex array object
		GLuint vbo; //Vertex buffer object
		GLuint ebo; //Element buffer object
	};
}

#endif //!GADGET_GL_MESH_INFO_H
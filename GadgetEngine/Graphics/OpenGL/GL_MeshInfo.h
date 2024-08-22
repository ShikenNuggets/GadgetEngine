#ifndef GADGET_GL_MESH_INFO_H
#define GADGET_GL_MESH_INFO_H

#include <glad/glad.h>

#include "Animation/AnimMesh.h"
#include "Animation/AnimVertex.h"
#include "Graphics/MeshInfo.h"
#include "Graphics/Vertex.h"

namespace Gadget{
	class GL_MeshInfo : public MeshInfo{
	public:
		GL_MeshInfo(const Submesh& mesh_);
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

	class GL_AnimMeshInfo : public MeshInfo{
	public:
		GL_AnimMeshInfo(const AnimSubmesh& mesh_);
		virtual ~GL_AnimMeshInfo() override;

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
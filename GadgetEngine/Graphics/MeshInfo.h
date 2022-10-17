#ifndef GADGET_MESH_INFO_H
#define GADGET_MESH_INFO_H

#include "Mesh.h"

namespace Gadget{
	class MeshInfo{
	public:
		MeshInfo(const Mesh& mesh_) : numIndices(mesh_.indices.size()){}
		virtual ~MeshInfo(){}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		inline constexpr size_t GetNumIndices() const{ return numIndices; }

	protected:
		size_t numIndices;
	};
}

#endif //!GADGET_MESH_INFO_H
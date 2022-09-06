#ifndef GADGET_MESH_H
#define GADGET_MESH_H

#include <vector>

#include "Vertex.h"

namespace Gadget{
	class Mesh{
	public:
		Mesh(const std::vector<Vertex>& verts_, const std::vector<uint32_t>& indices_) : vertices(verts_), indices(indices_){}

		const std::vector<Vertex> vertices;
		const std::vector<uint32_t> indices;
	};
}

#endif //!GADGET_MESH_H
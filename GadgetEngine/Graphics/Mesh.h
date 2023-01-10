#ifndef GADGET_MESH_H
#define GADGET_MESH_H

#include <vector>

#include "Debug.h"
#include "Vertex.h"
#include "Loaders/ObjLoader.h"
#include "Resource/Resource.h"

namespace Gadget{
	class Mesh : public Resource{
	public:
		Mesh(const std::vector<Vertex>& verts_, const std::vector<uint32_t>& indices_) : vertices(verts_), indices(indices_){}

		const std::vector<Vertex> vertices;
		const std::vector<uint32_t> indices;
	};

	class MeshResourceContainer : public ResourceContainer{
	public:
		MeshResourceContainer(const std::string& path_) : path(path_){}

		virtual Resource* LoadResource() override{
			if(loadCount == 0){
				GADGET_ASSERT(resource == nullptr, "Loaded resource has a reference count of 0!");
				resource = ObjLoader::LoadMesh(path);
			}

			GADGET_BASIC_ASSERT(resource != nullptr);
			loadCount++;
			return resource;
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(Mesh);
		}

	private:
		std::string path;
	};
}

#endif //!GADGET_MESH_H
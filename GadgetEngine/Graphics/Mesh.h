#ifndef GADGET_MESH_H
#define GADGET_MESH_H

#include <vector>

#include "Core/FileSystem.h"
#include "Debug.h"
#include "Vertex.h"
#include "Loaders/AssimpModelLoader.h"
#include "Resource/Resource.h"

namespace Gadget{
	struct Submesh{
		Submesh(const std::vector<Vertex>& verts_, const std::vector<uint32_t>& indices_) : vertices(verts_), indices(indices_){}

		const std::vector<Vertex> vertices;
		const std::vector<uint32_t> indices;
	};

	class Mesh : public Resource{
	public:
		Mesh(const std::vector<Submesh>& submeshes_) : submeshes(submeshes_){}
		Mesh(const std::vector<Vertex>& verts_, const std::vector<uint32_t>& indices_) : submeshes(1, Submesh(verts_, indices_)){}

		static constexpr const char* typeName = "Mesh";

		const std::vector<Submesh> submeshes;
	};

	class MeshResourceContainer : public ResourceContainer{
	public:
		MeshResourceContainer(const std::string& path_) : ResourceContainer(Mesh::typeName, path_){
			GADGET_BASIC_ASSERT(!path_.empty());
			GADGET_BASIC_ASSERT(FileSystem::FileExists(path_));
		}

		virtual Resource* LoadResource() override{
			return AssimpModelLoader::LoadMesh(path);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(Mesh);
		}
	};
}

#endif //!GADGET_MESH_H
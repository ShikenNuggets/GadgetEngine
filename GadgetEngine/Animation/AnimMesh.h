#ifndef GADGET_ANIMATION_ANIM_MESH_H
#define GADGET_ANIMATION_ANIM_MESH_H

#include <cstdint>

#include "Animation/AnimVertex.h"
#include "Animation/Skeleton.h"
#include "Core/FileSystem.h"
#include "Data/Array.h"
#include "Debug.h"
#include "Graphics/Vertex.h"
#include "Graphics/Loaders/AssimpModelLoader.h"
#include "Resource/Resource.h"

namespace Gadget{
	struct AnimSubmesh{
		AnimSubmesh(const Array<AnimVertex>& verts_, const Array<uint32_t>& indices_) : vertices(verts_), indices(indices_){}

		const Array<AnimVertex> vertices;
		const Array<uint32_t> indices;

		constexpr inline size_t SizeInBytes() const{ return vertices.SizeInBytes() + indices.SizeInBytes(); }
	};

	class AnimMesh : public Resource{
	public:
		AnimMesh(const Array<AnimSubmesh>& submeshes_, const Skeleton& skeleton_);

		static constexpr const char* typeName = "AnimMesh";

		virtual size_t SizeInBytes() const override{
			size_t size = sizeof(submeshes) + sizeof(skeleton);
			for(const auto& sm : submeshes){
				size += sm.SizeInBytes();
			}

			return size;
		}

		const Array<AnimSubmesh> submeshes;
		const Skeleton skeleton;
	};

	class AnimMeshResourceContainer : public ResourceContainer{
	public:
		AnimMeshResourceContainer(const std::string& path_) : ResourceContainer(AnimMesh::typeName, path_){}

		virtual Resource* LoadResource() override{
			return AssimpModelLoader::LoadAnimMesh(path);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(AnimMesh);
		}
	};
}

#endif //!GADGET_ANIMATION_ANIM_MESH_H
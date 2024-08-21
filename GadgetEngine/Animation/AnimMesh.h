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
		AnimSubmesh(const AnimSubmesh& other_) : vertices(other_.vertices), indices(other_.indices){}

		const Array<AnimVertex> vertices;
		const Array<uint32_t> indices;
	};

	class AnimMesh : public Resource{
	public:
		AnimMesh(const Array<AnimSubmesh>& submeshes_, const Skeleton& skeleton_, const Matrix4& globalInverse_) : submeshes(submeshes_), skeleton(skeleton_), globalInverse(globalInverse_){}

		static constexpr const char* typeName = "AnimMesh";

		const Array<AnimSubmesh> submeshes;
		const Skeleton skeleton;
		const Matrix4 globalInverse;
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
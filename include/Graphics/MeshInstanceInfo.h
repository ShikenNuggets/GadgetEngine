#ifndef GADGET_MESH_INSTANCE_INFO_H
#define GADGET_MESH_INSTANCE_INFO_H

#include "Math/Matrix.h"

namespace Gadget{
	//For storing info for a specific instance of a mesh
	//Most graphics APIs don't need this but it's very important for raytracing acceleration structures
	class MeshInstanceInfo{
	public:
		MeshInstanceInfo(const Matrix4& transform_){ Update(transform_); }
		virtual ~MeshInstanceInfo() = default;

		virtual void Update([[maybe_unused]] const Matrix4& transform_){}
	};
}

#endif //!GADGET_MESH_INSTANCE_INFO_H
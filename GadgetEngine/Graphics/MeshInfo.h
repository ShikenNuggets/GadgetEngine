#ifndef GADGET_MESH_INFO_H
#define GADGET_MESH_INFO_H

namespace Gadget{
	class MeshInfo{
	public:
		MeshInfo(){}
		virtual ~MeshInfo(){}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}

#endif //!GADGET_MESH_INFO_H
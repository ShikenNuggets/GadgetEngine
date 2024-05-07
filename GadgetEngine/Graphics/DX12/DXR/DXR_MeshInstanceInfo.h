#ifndef GADGET_DXR_MESH_INSTANCE_INFO_H
#define GADGET_DXR_MESH_INSTANCE_INFO_H

#include <DirectXMath.h>

#include "Graphics/MeshInstanceInfo.h"
#include "Graphics/DX12/DX12_Helpers.h"

namespace Gadget{
	class DXR_MeshInstanceInfo : public MeshInstanceInfo{
	public:
		DXR_MeshInstanceInfo(const Matrix4& transform_) : MeshInstanceInfo(transform_){
			transformMatrix = DX12_Helpers::ConvertMatrix4(transform_);
		}

		virtual ~DXR_MeshInstanceInfo() = default;

		virtual void Update(const Matrix4& transform_){
			MeshInstanceInfo::Update(transform_);
			transformMatrix = DX12_Helpers::ConvertMatrix4(transform_);
		}

		DirectX::XMMATRIX& DX12MatrixRef(){ return transformMatrix; }

	private:
		DirectX::XMMATRIX transformMatrix;
	};
}

#endif //!GADGET_DXR_MESH_INSTANCE_INFO_H
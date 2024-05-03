#ifndef GADGET_DXR_MESH_INFO_H
#define GADGET_DXR_MESH_INFO_H

#include <wrl/client.h>

#include "Graphics/MeshInfo.h"
#include "Graphics/DX12/DX12_Defines.h"

namespace Gadget{
	class DXR_MeshInfo : public MeshInfo{
	public:
		DXR_MeshInfo(const Mesh& mesh_);
		DXR_MeshInfo(size_t indexCount_, ID3D12_Resource* vertexBuffer_);
		virtual ~DXR_MeshInfo() override = default;

		virtual void Bind() override{}
		virtual void Unbind() override{}

		ID3D12_Resource* VertexBuffer(){ return vertexBuffer.Get(); }
		ID3D12_Resource* BottomLevelAS(){ return bottomLevelAS.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D12_Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12_Resource> bottomLevelAS;
	};
}

#endif //!GADGET_DXR_MESH_INFO_H
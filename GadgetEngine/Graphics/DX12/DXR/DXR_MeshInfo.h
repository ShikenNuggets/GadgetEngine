#ifndef GADGET_DXR_MESH_INFO_H
#define GADGET_DXR_MESH_INFO_H

#include <wrl/client.h>

#include "Graphics/MeshInfo.h"
#include "Graphics/DX12/DX12_Defines.h"
#include "Graphics/DX12/DXR/DXR_BottomLevelAS.h"

namespace Gadget{
	class DXR_MeshInfo : public MeshInfo{
	public:
		DXR_MeshInfo(const Mesh& mesh_);
		virtual ~DXR_MeshInfo() override;

		//TODO - For Testing Only
		DXR_MeshInfo(size_t indexCount_, ID3D12_Resource* vertexBuffer_, ID3D12_Resource* indexBuffer_);

		virtual void Bind() override{}
		virtual void Unbind() override{}

		ID3D12_Resource* VertexBuffer(){ return vertexBuffer.Get(); }
		ID3D12_Resource* IndexBuffer(){ return indexBuffer.Get(); }
		ID3D12_Resource* BottomLevelAS(){ return bottomLevelAS->Buffer(); }

		//TODO - For Testing Only
		inline void SetBottomLevelAS(DXR_BottomLevelAS* blas_){ bottomLevelAS = blas_; }

	private:
		Microsoft::WRL::ComPtr<ID3D12_Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12_Resource> indexBuffer;
		DXR_BottomLevelAS* bottomLevelAS;
	};
}

#endif //!GADGET_DXR_MESH_INFO_H
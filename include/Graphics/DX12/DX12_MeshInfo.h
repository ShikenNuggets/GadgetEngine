#ifndef GADGET_DX12_MESH_INFO_H
#define GADGET_DX12_MESH_INFO_H

#include <wrl/client.h>

#include "Graphics/MeshInfo.h"
#include "Graphics/DX12/DX12_Defines.h"

namespace Gadget{
	class DX12_MeshInfo : public MeshInfo{
	public:
		DX12_MeshInfo(const Submesh& mesh_);
		virtual ~DX12_MeshInfo() override;

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D12_Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12_Resource> indexBuffer;

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
		D3D12_PRIMITIVE_TOPOLOGY primitiveTopology;
	};
}

#endif //!GADGET_DX12_MESH_INFO_H
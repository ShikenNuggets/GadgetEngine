#ifndef GADGET_DXR_BOTTOM_LEVEL_AS_H
#define GADGET_DXR_BOTTOM_LEVEL_AS_H

#include <d3d12.h>
#include <wrl/client.h>

#include "Graphics/DX12/DX12_Defines.h"

namespace Gadget{
	class DXR_BottomLevelAS{
	public:
		DXR_BottomLevelAS(ID3D12_Resource* vertexBuffer_, size_t numVertices_, ID3D12_Resource* indexBuffer_, size_t numIndices_);
		~DXR_BottomLevelAS() = default;

		ID3D12_Resource* Buffer(){ return buffer.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D12_Resource> buffer;
	};
}

#endif //!GADGET_DXR_BOTTOM_LEVEL_AS_H
#ifndef GADGET_DXR_TOP_LEVEL_AS_H
#define GADGET_DXR_TOP_LEVEL_AS_H

#include <vector>

#include <d3d12.h>
#include <wrl/client.h>

#include "Graphics/DX12/DX12_Defines.h"
#include "Graphics/DX12/DXR/DXR_BottomLevelAS.h"
#include "Graphics/DX12/DXR/DXR_MaterialInfo.h"
#include "Graphics/DX12/DXR/DXR_MeshInfo.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/TopLevelASGenerator.h"
#include "Math/Matrix.h"

namespace Gadget{
	struct DXR_MeshInstance{
		DXR_MeshInstance(DXR_MeshInfo* meshInfo_, DXR_MaterialInfo* materialInfo_, const DirectX::XMMATRIX& transform_) : meshInfo(meshInfo_), materialInfo(materialInfo_), transform(transform_){}

		DXR_MeshInfo* meshInfo;
		DXR_MaterialInfo* materialInfo;
		DirectX::XMMATRIX transform;
	};

	class DXR_TopLevelAS{
		public:
			DXR_TopLevelAS(const std::vector<DXR_MeshInstance>& meshInstances_);
			~DXR_TopLevelAS() = default;

			ID3D12_Resource* Buffer(){ return mainBuffer.Get(); }

			void Regenerate(const std::vector<DXR_MeshInstance>& meshInstances_);
			void Update();

		private:
			Microsoft::WRL::ComPtr<ID3D12_Resource> mainBuffer;
			Microsoft::WRL::ComPtr<ID3D12_Resource> scratchBuffer;
			Microsoft::WRL::ComPtr<ID3D12_Resource> instanceDescBuffer;
			nv_helpers_dx12::TopLevelASGenerator topLevelASGenerator;
	};
}

#endif //!GADGET_DXR_TOP_LEVEL_AS_H
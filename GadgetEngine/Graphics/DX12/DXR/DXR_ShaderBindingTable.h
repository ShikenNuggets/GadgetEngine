#ifndef GADGET_DXR_SHADER_BINDING_TABLE_H
#define GADGET_DXR_SHADER_BINDING_TABLE_H

#include <wrl/client.h>

#include "Graphics/DX12/DX12_Defines.h"
#include "Graphics/DX12/DXR/DXR_PipelineStateObject.h"
#include "Graphics/DX12/DXR/DXR_ShaderResourceHeap.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/ShaderBindingTableGenerator.h"

namespace Gadget{
	struct HitGroupInfo{
		HitGroupInfo(size_t hitGroupIndex_, ID3D12_Resource* vertexBuffer_, ID3D12_Resource* indexBuffer_, ID3D12_Resource* constBuffer_) : hitGroupIndex(hitGroupIndex_){
			buffers.push_back(vertexBuffer_);
			buffers.push_back(indexBuffer_);
			buffers.push_back(constBuffer_);
		}

		size_t hitGroupIndex;
		std::vector<void*> buffers;
	};

	class DXR_ShaderBindingTable{
	public:
		DXR_ShaderBindingTable(DXR_PipelineStateObject* pso_, DXR_ShaderResourceHeap* heap_, const std::vector<HitGroupInfo>& hitGroupInfos_);

	private:
		nv_helpers_dx12::ShaderBindingTableGenerator sbtHelper;
		Microsoft::WRL::ComPtr<ID3D12Resource> sbtStorage;
	};
}

#endif //!GADGET_DXR_SHADER_BINDING_TABLE_H

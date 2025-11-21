#ifndef GADGET_DXR_SHADER_BINDING_TABLE_H
#define GADGET_DXR_SHADER_BINDING_TABLE_H

#include <wrl/client.h>

#include "Graphics/DX12/DX12_Defines.h"
#include "Graphics/DX12/DX12_DescriptorHeap.h"
#include "Graphics/DX12/DXR/DXR_PipelineStateObject.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/ShaderBindingTableGenerator.h"

namespace Gadget{
	struct HitGroupInfo{
		HitGroupInfo(size_t hitGroupIndex_, ID3D12_Resource* vertexBuffer_, ID3D12_Resource* indexBuffer_, ID3D12_Resource* constBuffer_) : hitGroupIndex(hitGroupIndex_){
			buffers.push_back(reinterpret_cast<void*>(vertexBuffer_->GetGPUVirtualAddress()));
			buffers.push_back(reinterpret_cast<void*>(indexBuffer_->GetGPUVirtualAddress()));
			buffers.push_back(reinterpret_cast<void*>(constBuffer_->GetGPUVirtualAddress()));
		}

		size_t hitGroupIndex;
		std::vector<void*> buffers;
	};

	class DXR_ShaderBindingTable{
	public:
		DXR_ShaderBindingTable(DXR_PipelineStateObject* pso_, DX12_DescriptorHeap* heap_, const std::vector<HitGroupInfo>& hitGroupInfos_);

		nv_helpers_dx12::ShaderBindingTableGenerator& SBTHelper(){ return sbtHelper; }
		ID3D12_Resource* Storage(){ return sbtStorage.Get(); }

	private:
		nv_helpers_dx12::ShaderBindingTableGenerator sbtHelper;
		Microsoft::WRL::ComPtr<ID3D12_Resource> sbtStorage;
	};
}

#endif //!GADGET_DXR_SHADER_BINDING_TABLE_H

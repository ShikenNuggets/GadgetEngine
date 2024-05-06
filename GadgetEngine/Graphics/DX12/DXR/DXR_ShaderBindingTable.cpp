#include "DXR_ShaderBindingTable.h"

#include "Graphics/DX12/DX12.h"
#include "Math/Vector.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

DXR_ShaderBindingTable::DXR_ShaderBindingTable(DXR_PipelineStateObject* pso_, DXR_ShaderResourceHeap* heap_, const std::vector<HitGroupInfo>& hitGroupInfos_){
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());
	GADGET_BASIC_ASSERT(pso_ != nullptr);
	GADGET_BASIC_ASSERT(heap_ != nullptr && heap_->Type() == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	sbtHelper.Reset();

	D3D12_GPU_DESCRIPTOR_HANDLE srvUavHeapHandle = heap_->GPUStart();
	UINT64* heapPointer = reinterpret_cast<UINT64*>(srvUavHeapHandle.ptr);

	sbtHelper.AddRayGenerationProgram(L"RayGen", { heapPointer });
	sbtHelper.AddMissProgram(L"Miss", {});
	sbtHelper.AddMissProgram(L"Miss", {}); //This is silly, but having (a multiple of) 2 miss programs honors an alignment requirement for the hit group table start address
	
	for(const auto& i : hitGroupInfos_){
		sbtHelper.AddHitGroup(pso_->HitGroupName(i.hitGroupIndex), i.buffers);
	}

	uint32_t sbtSize = sbtHelper.ComputeSBTSize();

	sbtStorage.Attach(DX12::GetInstance().CreateBuffer(nullptr, sbtSize, true, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_FLAG_NONE));
	if(!sbtStorage){
		Debug::ThrowFatalError(SID("RENDER"), "Could not allocate the shader binding table!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
	sbtStorage->SetName(L"Shader Binding Table Storage Buffer");

	sbtHelper.Generate(sbtStorage.Get(), pso_->Properties());
}

#include "DXR_TopLevelAS.h"

#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_Helpers.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

DXR_TopLevelAS::DXR_TopLevelAS(const std::vector<DXR_MeshInstance>& meshInstances_) : mainBuffer(nullptr), topLevelASGenerator(){
	Regenerate(meshInstances_);
}

void DXR_TopLevelAS::Regenerate(const std::vector<DXR_MeshInstance>& meshInstances_){
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());

	for(size_t i = 0; i < meshInstances_.size(); i++){
		topLevelASGenerator.AddInstance(meshInstances_[i].bottomLevelAS->Buffer(), DX12_Helpers::ConvertMatrix4(meshInstances_[i].transform), static_cast<UINT>(i), static_cast<UINT>(i));
	}

	uint64_t scratchSize = 0;
	uint64_t resultSize = 0;
	uint64_t instanceDescsSize = 0;
	topLevelASGenerator.ComputeASBufferSizes(DX12::GetInstance().MainDevice(), true, &scratchSize, &resultSize, &instanceDescsSize);

	scratchBuffer.Attach(DX12::GetInstance().CreateBuffer(nullptr, scratchSize, true, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));
	mainBuffer.Attach(DX12::GetInstance().CreateBuffer(nullptr, resultSize, false, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));
	instanceDescBuffer.Attach(DX12::GetInstance().CreateBuffer(nullptr, instanceDescsSize, true, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_FLAG_NONE));

	GADGET_BASIC_ASSERT(scratchBuffer != nullptr);
	GADGET_BASIC_ASSERT(mainBuffer != nullptr);
	GADGET_BASIC_ASSERT(instanceDescBuffer != nullptr);

	if(scratchBuffer != nullptr){
		scratchBuffer->SetName(L"TLAS_ScratchBuffer");
	}

	if(mainBuffer != nullptr){
		mainBuffer->SetName(L"TLAS_MainBuffer");
	}

	if(instanceDescBuffer != nullptr){
		instanceDescBuffer->SetName(L"TLAS_InstanceDescBuffer");
	}

	topLevelASGenerator.Generate(DX12::GetInstance().GfxCommand()->CommandList(), scratchBuffer.Get(), mainBuffer.Get(), instanceDescBuffer.Get(), false);
}

void DXR_TopLevelAS::Update(){
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());
	topLevelASGenerator.Generate(DX12::GetInstance().GfxCommand()->CommandList(), scratchBuffer.Get(), mainBuffer.Get(), instanceDescBuffer.Get(), true, mainBuffer.Get());
}
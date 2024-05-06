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
	for(size_t i = 0; i < meshInstances_.size(); i++){
		topLevelASGenerator.AddInstance(meshInstances_[i].bottomLevelAS->Buffer(), DX12_Helpers::ConvertMatrix4(meshInstances_[i].transform), static_cast<UINT>(i), static_cast<UINT>(i));
	}

	uint64_t scratchSize = 0;
	uint64_t resultSize = 0;
	uint64_t instanceDescsSize = 0;
	topLevelASGenerator.ComputeASBufferSizes(DX12::GetInstance().MainDevice(), true, &scratchSize, &resultSize, &instanceDescsSize);

	scratchBuffer.Attach(DX12_Helpers::CreateBuffer(DX12::GetInstance().MainDevice(), nullptr, scratchSize, true, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));
	mainBuffer.Attach(DX12_Helpers::CreateBuffer(DX12::GetInstance().MainDevice(), nullptr, resultSize, false, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));
	instanceDescBuffer.Attach(DX12_Helpers::CreateBuffer(DX12::GetInstance().MainDevice(), nullptr, instanceDescsSize, true, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_FLAG_NONE));

	topLevelASGenerator.Generate(DX12::GetInstance().GfxCommand()->CommandList(), scratchBuffer.Get(), mainBuffer.Get(), instanceDescBuffer.Get(), false);
}

void DXR_TopLevelAS::Update(){
	topLevelASGenerator.Generate(DX12::GetInstance().GfxCommand()->CommandList(), scratchBuffer.Get(), mainBuffer.Get(), instanceDescBuffer.Get(), true, mainBuffer.Get());
}
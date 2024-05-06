#include "DXR_ShaderResourceHeap.h"

#include "Graphics/DX12/DX12.h"

using namespace Gadget;

DXR_ShaderResourceHeap::DXR_ShaderResourceHeap(DXR_OutputResource* outputResource_, DXR_TopLevelAS* topLevelAS_) : DX12_DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV){
	GADGET_BASIC_ASSERT(outputResource_ != nullptr);
	GADGET_BASIC_ASSERT(topLevelAS_ != nullptr);

	Initialize(DX12::GetInstance().MainDevice(), 64, false);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	outputResourceHandle = DX12::GetInstance().CreateUAV(outputResource_->Resource(), nullptr, &uavDesc);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = topLevelAS_->Buffer()->GetGPUVirtualAddress();
	topLevelASHandle = DX12::GetInstance().CreateSRV(nullptr, &srvDesc);
}

DXR_ShaderResourceHeap::~DXR_ShaderResourceHeap(){
	for(auto& c : constantBufferViews){
		Free(c);
	}
	constantBufferViews.clear();

	Free(topLevelASHandle);
	Free(outputResourceHandle);
}

void DXR_ShaderResourceHeap::CreateCBV(ID3D12_Resource* buffer_, size_t size_){
	GADGET_BASIC_ASSERT(buffer_ != nullptr);
	GADGET_BASIC_ASSERT(size_ <= std::numeric_limits<UINT>::max());

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = buffer_->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = static_cast<UINT>(size_);
	constantBufferViews.push_back(DX12::GetInstance().CreateCBV(&cbvDesc));
}
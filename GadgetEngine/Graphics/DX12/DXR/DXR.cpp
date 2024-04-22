#include "DXR.h"

#include "Debug.h"
#include "Graphics/Vertex.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/BottomLevelASGenerator.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/RaytracingPipelineGenerator.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/RootSignatureGenerator.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

std::unique_ptr<DXR> DXR::instance = nullptr;

DXR::DXR(ScreenCoordinate frameSize_, ID3D12Resource* vertexBuffer_) : dx12(DX12::GetInstance()), frameSize(frameSize_), vertexBuffer(vertexBuffer_), srvUavHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV){
	GADGET_BASIC_ASSERT(frameSize_.x > 0);
	GADGET_BASIC_ASSERT(frameSize_.y > 0);
	GADGET_BASIC_ASSERT(vertexBuffer_ != nullptr);

	CreateAccelerationStructures();

	//dx12.GfxCommand()->CommandList()->Close();

	CreateRaytracingPipeline();
	CreateRaytracingOutputBuffer();
	CreateShaderResourceHeap();
	CreateShaderBindingTable();
}

DXR& DXR::GetInstance(){
	return *instance;
}

DXR& DXR::GetInstance(ScreenCoordinate frameSize_, ID3D12Resource* vertexBuffer_){
	if(instance == nullptr){
		instance = std::make_unique<DXR>(frameSize_, vertexBuffer_);
	}

	GADGET_ASSERT(instance != nullptr, "App instance was somehow nullptr! Nothing will work!");
	return *instance;
}

ErrorCode DXR::DeleteInstance(){
	if(instance != nullptr){
		instance.reset();
	}

	return ErrorCode::OK;
}

AccelerationStructureBuffers DXR::CreateBottomLevelAS(std::vector<std::pair<ComPtr<ID3D12Resource>, uint32_t>> vVertexBuffers_){
	nv_helpers_dx12::BottomLevelASGenerator blas;

	for(const auto& buffer : vVertexBuffers_){
		blas.AddVertexBuffer(buffer.first.Get(), 0, buffer.second, sizeof(Vertex), nullptr, 0);
	}

	uint64_t scratchSizeInBytes = 0;
	uint64_t resultSizeInBytes = 0;
	blas.ComputeASBufferSizes(dx12.MainDevice(), false, &scratchSizeInBytes, &resultSizeInBytes);

	AccelerationStructureBuffers buffers;
	buffers.pScratch = DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, scratchSizeInBytes, false, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	buffers.pResult = DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, resultSizeInBytes, false, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	blas.Generate(dx12.GfxCommand()->CommandList(), buffers.pScratch.Get(), buffers.pResult.Get(), false, nullptr);
	return buffers;
}

void DXR::CreateTopLevelAS(const std::vector<std::pair<ComPtr<ID3D12Resource>, DirectX::XMMATRIX>>& instances_){
	for(size_t i = 0; i < instances_.size(); i++){
		topLevelASGenerator.AddInstance(instances_[i].first.Get(), instances_[i].second, static_cast<UINT>(i), static_cast<UINT>(0));
	}

	uint64_t scratchSize = 0;
	uint64_t resultSize = 0;
	uint64_t instanceDescsSize = 0;
	topLevelASGenerator.ComputeASBufferSizes(dx12.MainDevice(), true, &scratchSize, &resultSize, &instanceDescsSize);

	topLevelASBuffers.pScratch = DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, scratchSize, true, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	topLevelASBuffers.pResult = DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, resultSize, true, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	topLevelASBuffers.pInstanceDesc = DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, instanceDescsSize, true, D3D12_RESOURCE_STATE_GENERIC_READ);

	topLevelASGenerator.Generate(dx12.GfxCommand()->CommandList(), topLevelASBuffers.pScratch.Get(), topLevelASBuffers.pResult.Get(), topLevelASBuffers.pInstanceDesc.Get());
}

void DXR::CreateAccelerationStructures(){
	(void)dx12.GfxCommand()->BeginFrame();

	AccelerationStructureBuffers bottomLevelBuffers = CreateBottomLevelAS({{ vertexBuffer.Get(), 3 }});
	instances = {{ bottomLevelBuffers.pResult, DirectX::XMMatrixIdentity() }};
	CreateTopLevelAS(instances);

	(void)dx12.GfxCommand()->EndFrame(nullptr);

	bottomLevelAS = bottomLevelBuffers.pResult;
}

ComPtr<ID3D12RootSignature> DXR::CreateRayGenSignature(){
	nv_helpers_dx12::RootSignatureGenerator rsg;
	rsg.AddHeapRangesParameter(
		{
			{ 0, 1, 0, D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0 },
			{ 0, 1, 0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1 }
		}
	);

	return rsg.Generate(dx12.MainDevice(), true);
}

ComPtr<ID3D12RootSignature> DXR::CreateMissSignature(){
	nv_helpers_dx12::RootSignatureGenerator rsg;
	return rsg.Generate(dx12.MainDevice(), true);
}

ComPtr<ID3D12RootSignature> DXR::CreateHitSignature(){
	nv_helpers_dx12::RootSignatureGenerator rsg;
	rsg.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_SRV);
	return rsg.Generate(dx12.MainDevice(), true);
}

void DXR::CreateRaytracingPipeline(){
	nv_helpers_dx12::RayTracingPipelineGenerator pipeline(dx12.MainDevice());

	//TODO
	//rayGenLibrary = nv_helpers_dx12::CompileShaderLibrary(L"RayGen.hlsl");
	//missLibrary = nv_helpers_dx12::CompileShaderLibrary(L"Miss.hlsl");
	//hitLibrary = nv_helpers_dx12::CompileShaderLibrary(L"Hit.hlsl");

	pipeline.AddLibrary(rayGenLibrary.Get(), { L"RayGen" });
	pipeline.AddLibrary(missLibrary.Get(), { L"Miss" });
	pipeline.AddLibrary(hitLibrary.Get(), { L"ClosestHit" });

	rayGenSignature = CreateRayGenSignature();
	missSignature = CreateMissSignature();
	hitSignature = CreateHitSignature();

	pipeline.AddHitGroup(L"HitGroup", L"ClosestHit");

	pipeline.AddRootSignatureAssociation(rayGenSignature.Get(), { L"RayGen" });
	pipeline.AddRootSignatureAssociation(missSignature.Get(), { L"Miss" });
	pipeline.AddRootSignatureAssociation(hitSignature.Get(), { L"HitGroup" });

	pipeline.SetMaxPayloadSize(4 * sizeof(float));
	pipeline.SetMaxAttributeSize(2 * sizeof(float));
	pipeline.SetMaxRecursionDepth(1);

	rtStateObject = pipeline.Generate();
	rtStateObject->QueryInterface(IID_PPV_ARGS(&rtStateObjectProperties));
}

void DXR::CreateRaytracingOutputBuffer(){
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.DepthOrArraySize = 1;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	resDesc.Width = frameSize.x;
	resDesc.Height = frameSize.y;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	dx12.MainDevice()->CreateCommittedResource(&DX12_Helpers::DefaultHeapProperties, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_COPY_SOURCE, nullptr, IID_PPV_ARGS(&outputResource));
}

void DXR::CreateShaderResourceHeap(){
	srvUavHeap.Initialize(dx12.MainDevice(), 2, true);
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvUavHeap.CPUStart();

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	dx12.MainDevice()->CreateUnorderedAccessView(outputResource.Get(), nullptr, &uavDesc, srvHandle);

	srvHandle.ptr += dx12.MainDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = topLevelASBuffers.pResult->GetGPUVirtualAddress();
	dx12.MainDevice()->CreateShaderResourceView(nullptr, &srvDesc, srvHandle);
}

void DXR::CreateShaderBindingTable(){
	sbtHelper.Reset();

	D3D12_GPU_DESCRIPTOR_HANDLE srvUavHeapHandle = srvUavHeap.GPUStart();
	auto heapPointer = reinterpret_cast<UINT64*>(srvUavHeapHandle.ptr);

	sbtHelper.AddRayGenerationProgram(L"RayGen", { heapPointer });
	sbtHelper.AddMissProgram(L"Miss", {});
	sbtHelper.AddHitGroup(L"HitGroup", { (void*)(vertexBuffer->GetGPUVirtualAddress()) });

	uint32_t sbtSize = sbtHelper.ComputeSBTSize();

	sbtStorage = DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, sbtSize, true, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_FLAG_NONE);
	if(!sbtStorage){
		throw std::logic_error("Could not allocate the shader binding table");
	}

	sbtHelper.Generate(sbtStorage.Get(), rtStateObjectProperties.Get());
}
#include "DXR.h"

#include "Debug.h"
#include "Graphics/Vertex.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_ShaderHandler.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/BottomLevelASGenerator.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/RaytracingPipelineGenerator.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/RootSignatureGenerator.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

std::unique_ptr<DXR> DXR::instance = nullptr;

DXR::DXR(ScreenCoordinate frameSize_, const std::vector<DXR_MeshInfo*>& meshInfo_) : dx12(DX12::GetInstance()), frameSize(frameSize_), meshInfos(meshInfo_){
	GADGET_BASIC_ASSERT(frameSize_.x > 0);
	GADGET_BASIC_ASSERT(frameSize_.y > 0);
	GADGET_BASIC_ASSERT(meshInfo_.size() > 0);

	for(size_t i = 0; i < meshInfo_.size(); i++){
		GADGET_BASIC_ASSERT(meshInfo_[i] != nullptr);
	}

	CreateAccelerationStructures({});

	auto err = dx12.GfxCommand()->CloseList();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Could not close command list after creating acceleration structures!", err, __FILE__, __LINE__);
	}

	CreateRaytracingPipeline();
	CreateRaytracingOutputBuffer();

	CreateCameraBuffer();

	CreateShaderResourceHeap();
	CreateShaderBindingTable();
}

DXR& DXR::GetInstance(){
	return *instance;
}

DXR& DXR::GetInstance(ScreenCoordinate frameSize_, const std::vector<DXR_MeshInfo*>& meshInfo_){
	GADGET_BASIC_ASSERT(frameSize_.x > 0 && frameSize_.y > 0);
	GADGET_BASIC_ASSERT(meshInfo_.size() > 0);
	for(size_t i = 0; i < meshInfo_.size(); i++){
		GADGET_BASIC_ASSERT(meshInfo_[i] != nullptr);
	}

	if(instance == nullptr){
		instance = std::make_unique<DXR>(frameSize_, meshInfo_);
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

AccelerationStructureBuffers DXR::CreateBottomLevelAS(std::vector<std::pair<ComPtr<ID3D12Resource>, uint32_t>> vVertexBuffers_, std::vector<std::pair<Microsoft::WRL::ComPtr<ID3D12Resource>, uint32_t>> vIndexBuffers_){
	nv_helpers_dx12::BottomLevelASGenerator blas;

	for(size_t i = 0; i < vVertexBuffers_.size(); i++){
		if(i < vIndexBuffers_.size() && vIndexBuffers_[i].second > 0){
			blas.AddVertexBuffer(vVertexBuffers_[i].first.Get(), 0, vVertexBuffers_[i].second, 28, vIndexBuffers_[i].first.Get(), 0, vIndexBuffers_[i].second, nullptr, 0, true);
		}else{
			blas.AddVertexBuffer(vVertexBuffers_[i].first.Get(), 0, vVertexBuffers_[i].second, 28, nullptr, 0);
		}
	}

	uint64_t scratchSizeInBytes = 0;
	uint64_t resultSizeInBytes = 0;
	blas.ComputeASBufferSizes(dx12.MainDevice(), false, &scratchSizeInBytes, &resultSizeInBytes);

	AccelerationStructureBuffers buffers;
	buffers.pScratch.Attach(DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, scratchSizeInBytes, false, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));
	buffers.pResult.Attach(DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, resultSizeInBytes, false, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));

	blas.Generate(dx12.GfxCommand()->CommandList(), buffers.pScratch.Get(), buffers.pResult.Get(), false, nullptr);
	return buffers;
}

void DXR::CreateTopLevelAS(const std::vector<std::pair<ComPtr<ID3D12Resource>, DirectX::XMMATRIX>>& instances_, bool updateOnly_){
	if(!updateOnly_){
		for(size_t i = 0; i < instances_.size(); i++){
			topLevelASGenerator.AddInstance(instances_[i].first.Get(), instances_[i].second, static_cast<UINT>(i), static_cast<UINT>(i));
		}

		uint64_t scratchSize = 0;
		uint64_t resultSize = 0;
		uint64_t instanceDescsSize = 0;
		topLevelASGenerator.ComputeASBufferSizes(dx12.MainDevice(), true, &scratchSize, &resultSize, &instanceDescsSize);

		topLevelASBuffers.pScratch.Attach(DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, scratchSize, true, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));
		topLevelASBuffers.pResult.Attach(DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, resultSize, false, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));
		topLevelASBuffers.pInstanceDesc.Attach(DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, instanceDescsSize, true, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_FLAG_NONE));
	}

	topLevelASGenerator.Generate(dx12.GfxCommand()->CommandList(), topLevelASBuffers.pScratch.Get(), topLevelASBuffers.pResult.Get(), topLevelASBuffers.pInstanceDesc.Get(), updateOnly_, topLevelASBuffers.pResult.Get());
}

void DXR::UpdateTopLevelAS(){
	CreateTopLevelAS({}, true);
}

void DXR::CreateAccelerationStructures(const std::vector<ComPtr<ID3D12_Resource>>& resources_){
	auto* cmdList = dx12.GfxCommand()->CommandList();
	GADGET_BASIC_ASSERT(cmdList != nullptr);

	std::vector<AccelerationStructureBuffers> bottomLevelBuffers;
	bottomLevelBuffers.push_back(CreateBottomLevelAS({{ meshInfos[0]->VertexBuffer(), 3}}, {{ meshInfos[0]->IndexBuffer(), static_cast<uint32_t>(meshInfos[0]->GetNumIndices()) }}));
	bottomLevelBuffers.push_back(CreateBottomLevelAS({{ meshInfos[1]->VertexBuffer(), 4}}, {{ meshInfos[1]->IndexBuffer(), static_cast<uint32_t>(meshInfos[1]->GetNumIndices()) }}));

	bottomLevelBuffers[0].SetName(L"Triangle BLAS");
	bottomLevelBuffers[1].SetName(L"Plane BLAS");

	instances = {
		{ bottomLevelBuffers[0].pResult, DirectX::XMMatrixIdentity()},
		{ bottomLevelBuffers[0].pResult, DirectX::XMMatrixTranslation(-0.6f, 0.0f, 0.0f) },
		{ bottomLevelBuffers[0].pResult, DirectX::XMMatrixTranslation(0.6f, 0.0f, 0.0f) },

		{ bottomLevelBuffers[1].pResult, DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f) },
	};
	CreateTopLevelAS(instances);
	topLevelASBuffers.SetName(L"TLAS");

	auto err = dx12.GfxCommand()->ExecuteCommandsImmediate();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Could not execute commands for creating acceleration structures!", err, __FILE__, __LINE__);
	}

	bottomLevelAS = bottomLevelBuffers[0].pResult;
	bottomLevelAS2 = bottomLevelBuffers[1].pResult;
}

ID3D12RootSignature* DXR::CreateRayGenSignature(){
	nv_helpers_dx12::RootSignatureGenerator rsg;
	rsg.AddHeapRangesParameter(
		{
			{ 0, 1, 0, D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0 }, //Output buffer
			{ 0, 1, 0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1 }, //Top-level acceleration structure
			{ 0, 1, 0, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 2 }, //Camera parameters
		}
	);

	ID3D12RootSignature* rootSig = rsg.Generate(dx12.MainDevice(), true);
	if(rootSig == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create ray generation signature!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}

	rootSig->SetName(L"RayGen Root Signature");
	return rootSig;
}

ID3D12RootSignature* DXR::CreateMissSignature(){
	nv_helpers_dx12::RootSignatureGenerator rsg;

	ID3D12RootSignature* rootSig = rsg.Generate(dx12.MainDevice(), true);
	if(rootSig == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create miss signature!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}

	rootSig->SetName(L"Miss Root Signature");
	return rootSig;
}

ID3D12RootSignature* DXR::CreateHitSignature(){
	nv_helpers_dx12::RootSignatureGenerator rsg;
	rsg.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_SRV, 0); //Vertices
	rsg.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_SRV, 1); //Indices
	rsg.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 0); //Colors

	ID3D12RootSignature* rootSig = rsg.Generate(dx12.MainDevice(), true);
	if(rootSig == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create hit signature!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}

	rootSig->SetName(L"Hit Root Signature");
	return rootSig;
}

void DXR::CreateRaytracingPipeline(){
	nv_helpers_dx12::RayTracingPipelineGenerator pipeline(dx12.MainDevice());

	rayGenLibrary = DX12_ShaderHandler::GetEngineShader(EngineShader::ID::RayGen_Lib);
	missLibrary = DX12_ShaderHandler::GetEngineShader(EngineShader::ID::Miss_Lib);
	hitLibrary = DX12_ShaderHandler::GetEngineShader(EngineShader::ID::Hit_Lib);

	GADGET_BASIC_ASSERT(rayGenLibrary.BytecodeLength > 0 && rayGenLibrary.pShaderBytecode != nullptr);
	GADGET_BASIC_ASSERT(missLibrary.BytecodeLength > 0 && missLibrary.pShaderBytecode != nullptr);
	GADGET_BASIC_ASSERT(hitLibrary.BytecodeLength > 0 && hitLibrary.pShaderBytecode != nullptr);

	pipeline.AddLibrary(rayGenLibrary, { L"RayGen" });
	pipeline.AddLibrary(missLibrary, { L"Miss" });
	pipeline.AddLibrary(hitLibrary, { L"ClosestHit", L"PlaneClosestHit" });

	rayGenSignature.Attach(CreateRayGenSignature());
	missSignature.Attach(CreateMissSignature());
	hitSignature.Attach(CreateHitSignature());

	GADGET_BASIC_ASSERT(rayGenSignature != nullptr);
	GADGET_BASIC_ASSERT(missSignature != nullptr);
	GADGET_BASIC_ASSERT(hitSignature != nullptr);

	pipeline.AddHitGroup(L"HitGroup", L"ClosestHit");
	pipeline.AddHitGroup(L"PlaneHitGroup", L"PlaneClosestHit");

	pipeline.AddRootSignatureAssociation(rayGenSignature.Get(), { L"RayGen" });
	pipeline.AddRootSignatureAssociation(missSignature.Get(), { L"Miss" });
	pipeline.AddRootSignatureAssociation(hitSignature.Get(), { L"HitGroup", L"PlaneHitGroup"});

	pipeline.SetMaxPayloadSize(4 * sizeof(float));
	pipeline.SetMaxAttributeSize(2 * sizeof(float));
	pipeline.SetMaxRecursionDepth(1);

	rtStateObject.Attach(pipeline.Generate());
	if(rtStateObject == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not generate raytracing pipeline!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
	rtStateObject->SetName(L"Raytracing Pipeline State Object");

	HRESULT hr = rtStateObject->QueryInterface(IID_PPV_ARGS(rtStateObjectProperties.ReleaseAndGetAddressOf()));
	if(FAILED(hr) || rtStateObjectProperties == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not query interface for rtStateObject!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
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

	HRESULT hr = dx12.MainDevice()->CreateCommittedResource(&DX12_Helpers::DefaultHeapProperties, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_COPY_SOURCE, nullptr, IID_PPV_ARGS(outputResource.ReleaseAndGetAddressOf()));
	if(FAILED(hr) || outputResource == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create output resource!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
	
	outputResource->SetName(L"DXR Output Resource");
}

void DXR::CreateShaderResourceHeap(){
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = dx12.SRVHeap().CPUStart();

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	dx12.MainDevice()->CreateUnorderedAccessView(outputResource.Get(), nullptr, &uavDesc, srvHandle);

	srvHandle.ptr += dx12.SRVHeap().DescriptorSize();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = topLevelASBuffers.pResult->GetGPUVirtualAddress();
	dx12.MainDevice()->CreateShaderResourceView(nullptr, &srvDesc, srvHandle);

	srvHandle.ptr += dx12.SRVHeap().DescriptorSize();

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = cameraBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = cameraBufferSize; 
	dx12.MainDevice()->CreateConstantBufferView(&cbvDesc, srvHandle);
}

void DXR::CreateShaderBindingTable(){
	//Test Stuff
	DirectX::XMVECTOR bufferDataA[] = {
		// A
		DirectX::XMVECTOR{1.0f, 0.0f, 0.0f, 1.0f},
		DirectX::XMVECTOR{1.0f, 0.4f, 0.0f, 1.0f},
		DirectX::XMVECTOR{1.f, 0.7f, 0.0f, 1.0f},
	};

	DirectX::XMVECTOR bufferDataB[] = {
		// B
		DirectX::XMVECTOR{0.0f, 1.0f, 0.0f, 1.0f},
		DirectX::XMVECTOR{0.0f, 1.0f, 0.4f, 1.0f},
		DirectX::XMVECTOR{0.0f, 1.0f, 0.7f, 1.0f},
	};

	DirectX::XMVECTOR bufferDataC[] = {
		// C
		DirectX::XMVECTOR{0.0f, 0.0f, 1.0f, 1.0f},
		DirectX::XMVECTOR{0.4f, 0.0f, 1.0f, 1.0f},
		DirectX::XMVECTOR{0.7f, 0.0f, 1.0f, 1.0f},
	};

	colorConstBuffers.clear();
	colorConstBuffers = std::vector<ComPtr<ID3D12Resource>>(3);

	colorConstBuffers[0].Attach(DX12_Helpers::CreateBuffer(dx12.MainDevice(), bufferDataA, sizeof(bufferDataA), true, D3D12_RESOURCE_STATE_GENERIC_READ));
	colorConstBuffers[1].Attach(DX12_Helpers::CreateBuffer(dx12.MainDevice(), bufferDataB, sizeof(bufferDataB), true, D3D12_RESOURCE_STATE_GENERIC_READ));
	colorConstBuffers[2].Attach(DX12_Helpers::CreateBuffer(dx12.MainDevice(), bufferDataC, sizeof(bufferDataC), true, D3D12_RESOURCE_STATE_GENERIC_READ));
	//End Test Stuff

	sbtHelper.Reset();

	D3D12_GPU_DESCRIPTOR_HANDLE srvUavHeapHandle = dx12.SRVHeap().GPUStart();
	UINT64* heapPointer = reinterpret_cast<UINT64*>(srvUavHeapHandle.ptr);

	sbtHelper.AddRayGenerationProgram(L"RayGen", { heapPointer });
	sbtHelper.AddMissProgram(L"Miss", {});
	sbtHelper.AddMissProgram(L"Miss", {}); //This is silly, but having (a multiple of) 2 miss programs honors an alignment requirement for the hit group table start address
	for(int i = 0; i < 3; i++){
		sbtHelper.AddHitGroup(L"HitGroup", { (void*)(meshInfos[0]->VertexBuffer()->GetGPUVirtualAddress()), (void*)(meshInfos[0]->IndexBuffer()->GetGPUVirtualAddress()), (void*)(colorConstBuffers[i]->GetGPUVirtualAddress()) });
	}

	sbtHelper.AddHitGroup(L"PlaneHitGroup", { (void*)(meshInfos[1]->VertexBuffer()->GetGPUVirtualAddress()), (void*)(meshInfos[1]->IndexBuffer()->GetGPUVirtualAddress()), (void*)(colorConstBuffers[0]->GetGPUVirtualAddress()) });

	uint32_t sbtSize = sbtHelper.ComputeSBTSize();

	sbtStorage.Attach(DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, sbtSize, true, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_FLAG_NONE));
	if(!sbtStorage){
		Debug::ThrowFatalError(SID("RENDER"), "Could not allocate the shader binding table!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
	sbtStorage->SetName(L"Shader Binding Table Storage Buffer");

	sbtHelper.Generate(sbtStorage.Get(), rtStateObjectProperties.Get());
}

constexpr uint32_t numMatrices = 2; //View inverse, projection inverse

void DXR::CreateCameraBuffer(){
	cameraBufferSize = static_cast<uint32_t>(Utils::AlignSizeUp<256>(numMatrices * sizeof(DirectX::XMMATRIX))); //TODO - Apparently the device requires SizeInBytes be a multiple of 256. What if that changes?

	cameraBuffer.Attach(DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, cameraBufferSize, true, D3D12_RESOURCE_STATE_GENERIC_READ));
	if(cameraBuffer == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create buffer for camera properties!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
	cameraBuffer->SetName(L"Camera Buffer");

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = cameraBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = cameraBufferSize;

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = dx12.SRVHeap().CPUStart();
	dx12.MainDevice()->CreateConstantBufferView(&cbvDesc, srvHandle);
}

void DXR::UpdateCameraBuffer(const Matrix4& view_, const Matrix4& perspective_){
	GADGET_BASIC_ASSERT(cameraBuffer != nullptr);

	std::vector<Matrix4> matrices;
	matrices.push_back(view_.Inverse());
	matrices.push_back(perspective_.Inverse());

	GADGET_BASIC_ASSERT(sizeof(Matrix4) * matrices.size() <= cameraBufferSize);

	DX12_Helpers::UpdateBuffer(cameraBuffer.Get(), matrices.data(), sizeof(Matrix4) * matrices.size());
}
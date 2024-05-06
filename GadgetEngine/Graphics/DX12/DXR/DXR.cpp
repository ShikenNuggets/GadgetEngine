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

DXR::DXR(ScreenCoordinate frameSize_, const std::vector<DXR_MeshInfo*>& meshInfo_) : dx12(DX12::GetInstance()), meshInfos(meshInfo_), cameraBufferHandle(), topLevelASHandle(), outputResourceHandle(){
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

	pso = new DXR_PipelineStateObject();
	outputResource = new DXR_OutputResource(frameSize_);

	CreateCameraBuffer();

	CreateShaderResourceHeap();
	CreateShaderBindingTable();
}

DXR::~DXR(){
	dx12.SRVHeap().Free(cameraBufferHandle);
	dx12.SRVHeap().Free(topLevelASHandle);
	dx12.SRVHeap().Free(outputResourceHandle);

	delete outputResource;
	outputResource = nullptr;

	delete pso;
	pso = nullptr;

	delete topLevelAS;
	topLevelAS = nullptr;
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

void DXR::UpdateTopLevelAS(){
	topLevelAS->Update();
}

void DXR::CreateAccelerationStructures(const std::vector<ComPtr<ID3D12_Resource>>& resources_){
	auto* cmdList = dx12.GfxCommand()->CommandList();
	GADGET_BASIC_ASSERT(cmdList != nullptr);

	DXR_BottomLevelAS* triangleBLAS = new DXR_BottomLevelAS(meshInfos[0]->VertexBuffer(), 3, meshInfos[0]->IndexBuffer(), meshInfos[0]->GetNumIndices());
	DXR_BottomLevelAS* planeBLAS = new DXR_BottomLevelAS(meshInfos[1]->VertexBuffer(), 4, meshInfos[1]->IndexBuffer(), meshInfos[1]->GetNumIndices());

	meshInfos[0]->SetBottomLevelAS(triangleBLAS);
	meshInfos[1]->SetBottomLevelAS(planeBLAS);

	instances.clear();
	instances.reserve(4);
	instances.push_back(DXR_MeshInstance(triangleBLAS, DX12_Helpers::ConvertMatrix4(Matrix4::Identity())));
	instances.push_back(DXR_MeshInstance(triangleBLAS, DX12_Helpers::ConvertMatrix4(Matrix4::Translate(Vector3(-0.6f, 0.0f, 0.0f)))));
	instances.push_back(DXR_MeshInstance(triangleBLAS, DX12_Helpers::ConvertMatrix4(Matrix4::Translate(Vector3(0.6f, 0.0f, 0.0f)))));
	instances.push_back(DXR_MeshInstance(planeBLAS, DX12_Helpers::ConvertMatrix4(Matrix4::Identity())));
	instances.shrink_to_fit();

	topLevelAS = new DXR_TopLevelAS(instances);

	auto err = dx12.GfxCommand()->ExecuteCommandsImmediate();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Could not execute commands for creating acceleration structures!", err, __FILE__, __LINE__);
	}

	triangleBLAS->ReleaseTempResources();
	planeBLAS->ReleaseTempResources();
}

void DXR::CreateShaderResourceHeap(){
	GADGET_BASIC_ASSERT(outputResource != nullptr);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	outputResourceHandle = dx12.CreateUAV(outputResource->Resource(), nullptr, &uavDesc);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = topLevelAS->Buffer()->GetGPUVirtualAddress();
	topLevelASHandle = dx12.CreateSRV(nullptr, &srvDesc);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = cameraBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = cameraBufferSize;
	cameraBufferHandle = dx12.CreateCBV(&cbvDesc);
}

void DXR::CreateShaderBindingTable(){
	GADGET_BASIC_ASSERT(pso != nullptr);

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

	sbtHelper.Generate(sbtStorage.Get(), pso->Properties());
}

constexpr uint32_t numMatrices = 2; //View inverse, projection inverse

void DXR::CreateCameraBuffer(){
	cameraBufferSize = static_cast<uint32_t>(Utils::AlignSizeUp<256>(numMatrices * sizeof(DirectX::XMMATRIX))); //TODO - Apparently the device requires SizeInBytes be a multiple of 256. What if that changes?

	cameraBuffer.Attach(DX12_Helpers::CreateBuffer(dx12.MainDevice(), nullptr, cameraBufferSize, true, D3D12_RESOURCE_STATE_GENERIC_READ));
	if(cameraBuffer == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create buffer for camera properties!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}

	cameraBuffer->SetName(L"Camera Buffer");
}

void DXR::UpdateCameraBuffer(const Matrix4& view_, const Matrix4& perspective_){
	GADGET_BASIC_ASSERT(cameraBuffer != nullptr);

	std::vector<Matrix4> matrices;
	matrices.push_back(view_.Inverse());
	matrices.push_back(perspective_.Inverse());

	GADGET_BASIC_ASSERT(sizeof(Matrix4) * matrices.size() <= cameraBufferSize);

	DX12_Helpers::UpdateBuffer(cameraBuffer.Get(), matrices.data(), sizeof(Matrix4) * matrices.size());
}
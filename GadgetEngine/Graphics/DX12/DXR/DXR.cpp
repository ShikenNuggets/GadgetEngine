#include "DXR.h"

#include "Debug.h"
#include "Graphics/Vertex.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_ShaderHandler.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

std::unique_ptr<DXR> DXR::instance = nullptr;

DXR::DXR(ScreenCoordinate frameSize_, const std::vector<DXR_MeshInfo*>& meshInfo_) : dx12(DX12::GetInstance()), meshInfos(meshInfo_){
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
	delete shaderBindingTable;
	shaderBindingTable = nullptr;

	if(heap != nullptr){
		heap->Release();
		delete heap;
		heap = nullptr;
	}

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
	GADGET_BASIC_ASSERT(topLevelAS != nullptr);
	GADGET_BASIC_ASSERT(cameraBuffer != nullptr);

	heap = new DXR_ShaderResourceHeap(outputResource, topLevelAS);
	heap->CreateCBV(cameraBuffer.Get(), cameraBufferSize);
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

	std::vector<HitGroupInfo> hitGroupInfos;
	hitGroupInfos.push_back(HitGroupInfo(0, meshInfos[0]->VertexBuffer(), meshInfos[0]->IndexBuffer(), colorConstBuffers[0].Get()));
	hitGroupInfos.push_back(HitGroupInfo(0, meshInfos[0]->VertexBuffer(), meshInfos[0]->IndexBuffer(), colorConstBuffers[1].Get()));
	hitGroupInfos.push_back(HitGroupInfo(0, meshInfos[0]->VertexBuffer(), meshInfos[0]->IndexBuffer(), colorConstBuffers[2].Get()));
	hitGroupInfos.push_back(HitGroupInfo(1, meshInfos[1]->VertexBuffer(), meshInfos[1]->IndexBuffer(), colorConstBuffers[0].Get()));

	shaderBindingTable = new DXR_ShaderBindingTable(pso, heap, hitGroupInfos);
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
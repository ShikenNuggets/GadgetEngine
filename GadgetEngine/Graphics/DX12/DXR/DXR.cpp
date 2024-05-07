#include "DXR.h"

#include "Debug.h"
#include "Graphics/Vertex.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_ShaderHandler.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

std::unique_ptr<DXR> DXR::instance = nullptr;

DXR::DXR(ScreenCoordinate frameSize_, const std::vector<DXR_MeshInfo*>& meshInfo_) : dx12(DX12::GetInstance()), meshInfos(meshInfo_), topLevelAS(nullptr), pso(nullptr), outputResource(nullptr), heap(nullptr), shaderBindingTable(nullptr){
	GADGET_BASIC_ASSERT(frameSize_.x > 0);
	GADGET_BASIC_ASSERT(frameSize_.y > 0);

	for(size_t i = 0; i < meshInfo_.size(); i++){
		GADGET_BASIC_ASSERT(meshInfo_[i] != nullptr);
	}

	pso = new DXR_PipelineStateObject();
	outputResource = new DXR_OutputResource(frameSize_);

	CreateCameraBuffer();
	CreateTopLevelAS({});

	auto err = dx12.GfxCommand()->CloseList();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Could not close command list after creating acceleration structures!", err, __FILE__, __LINE__);
	}
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

void DXR::CreateTopLevelAS(const std::vector<DXR_MeshInstance>& meshInstances_){
	auto* cmdList = dx12.GfxCommand()->CommandList();
	GADGET_BASIC_ASSERT(cmdList != nullptr);

	instances = meshInstances_;

	if(topLevelAS == nullptr){
		topLevelAS = new DXR_TopLevelAS(instances);
	}else{
		topLevelAS->Regenerate(instances);
	}

	auto err = dx12.GfxCommand()->ExecuteCommandsImmediate();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Could not execute commands for creating acceleration structures!", err, __FILE__, __LINE__);
	}

	CreateShaderResourceHeap();
	CreateShaderBindingTable();
}

void DXR::CreateShaderResourceHeap(){
	GADGET_BASIC_ASSERT(outputResource != nullptr);
	GADGET_BASIC_ASSERT(topLevelAS != nullptr);
	GADGET_BASIC_ASSERT(cameraBuffer != nullptr);

	if(heap != nullptr){
		delete heap;
		heap = nullptr;
	}

	heap = new DXR_ShaderResourceHeap(outputResource, topLevelAS);
	heap->CreateCBV(cameraBuffer.Get(), cameraBufferSize);
}

void DXR::CreateShaderBindingTable(){
	GADGET_BASIC_ASSERT(pso != nullptr);

	std::vector<HitGroupInfo> hitGroupInfos;
	for(const auto& i : instances){
		GADGET_BASIC_ASSERT(i.meshInfo != nullptr);
		GADGET_BASIC_ASSERT(i.materialInfo != nullptr);
		hitGroupInfos.push_back(HitGroupInfo(i.materialInfo->HitGroupIndex(), i.meshInfo->VertexBuffer(), i.meshInfo->IndexBuffer(), i.materialInfo->ConstBuffer()));
	}

	shaderBindingTable = new DXR_ShaderBindingTable(pso, heap, hitGroupInfos);
}

constexpr uint32_t numMatrices = 2; //View inverse, projection inverse

void DXR::CreateCameraBuffer(){
	cameraBufferSize = static_cast<uint32_t>(Utils::AlignSizeUp<256>(numMatrices * sizeof(Matrix4))); //TODO - Apparently the device requires SizeInBytes be a multiple of 256. What if that changes?

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
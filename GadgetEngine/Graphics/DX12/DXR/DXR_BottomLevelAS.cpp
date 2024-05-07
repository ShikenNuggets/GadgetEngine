#include "DXR_BottomLevelAS.h"

#include "Graphics/Vertex.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/BottomLevelASGenerator.h"
#include "Utils/StringID.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

DXR_BottomLevelAS::DXR_BottomLevelAS(ID3D12_Resource* vertexBuffer_, size_t numVertices_, ID3D12_Resource* indexBuffer_, size_t numIndices_) : mainBuffer(nullptr), scratchBuffer(nullptr){
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());
	GADGET_BASIC_ASSERT(vertexBuffer_ != nullptr);
	GADGET_BASIC_ASSERT(numVertices_ > 0);
	GADGET_BASIC_ASSERT(numVertices_ <= std::numeric_limits<uint32_t>::max());
	GADGET_BASIC_ASSERT(numIndices_ <= std::numeric_limits<uint32_t>::max());

	nv_helpers_dx12::BottomLevelASGenerator blas;

	if(indexBuffer_ != nullptr && numIndices_ > 0){
		blas.AddVertexBuffer(vertexBuffer_, 0, static_cast<uint32_t>(numVertices_), sizeof(Vertex), indexBuffer_, 0, static_cast<uint32_t>(numIndices_), nullptr, 0, true);
	}else{
		blas.AddVertexBuffer(vertexBuffer_, 0, static_cast<uint32_t>(numVertices_), sizeof(Vertex), nullptr, 0);
	}

	uint64_t scratchSizeInBytes = 0;
	uint64_t resultSizeInBytes = 0;
	blas.ComputeASBufferSizes(DX12::GetInstance().MainDevice(), false, &scratchSizeInBytes, &resultSizeInBytes);

	scratchBuffer.Attach(DX12::GetInstance().CreateBuffer(nullptr, scratchSizeInBytes, false, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));
	mainBuffer.Attach(DX12::GetInstance().CreateBuffer(nullptr, resultSizeInBytes, false, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));
	
	GADGET_BASIC_ASSERT(scratchBuffer != nullptr);
	GADGET_BASIC_ASSERT(mainBuffer != nullptr);

	if(scratchBuffer != nullptr){
		scratchBuffer->SetName(L"BLAS_ScratchBuffer");
	}

	if(mainBuffer != nullptr){
		mainBuffer->SetName(L"BLAS_MainBuffer");
	}

	blas.Generate(DX12::GetInstance().GfxCommand()->CommandList(), scratchBuffer.Get(), mainBuffer.Get(), false, nullptr);
}

DXR_BottomLevelAS::~DXR_BottomLevelAS(){
	DX12::GetInstance().DeferredRelease(mainBuffer.Detach());
}

void DXR_BottomLevelAS::ReleaseTempResources(){
	scratchBuffer.Reset();
}
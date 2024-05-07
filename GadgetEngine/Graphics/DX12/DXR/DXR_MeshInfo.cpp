#include "DXR_MeshInfo.h"

#include "Random.h"
#include "Graphics/Color.h"
#include "Graphics/Vertex.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_Helpers.h"
#include "Graphics/DX12/DXR/DXR.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

DXR_MeshInfo::DXR_MeshInfo(const Mesh& mesh_) : MeshInfo(mesh_.indices.size()), vertexBuffer(nullptr), indexBuffer(nullptr), bottomLevelAS(nullptr){
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());
	GADGET_BASIC_ASSERT(mesh_.vertices.size() > 0);
	GADGET_BASIC_ASSERT(mesh_.indices.size() > 0);
	GADGET_BASIC_ASSERT(mesh_.vertices.size() < mesh_.indices.size()); //There should always be more indices than vertices
	GADGET_BASIC_ASSERT(mesh_.indices.size() <= std::numeric_limits<uint32_t>::max()); //If you need more indices, adjust the index buffer creation

	//Vertex Buffer
	vertexBuffer.Attach(DX12::GetInstance().CreateBuffer(mesh_.vertices.data(), mesh_.vertices.size() * sizeof(Vertex), true, D3D12_RESOURCE_STATE_GENERIC_READ));
	GADGET_BASIC_ASSERT(vertexBuffer != nullptr);
	if(vertexBuffer != nullptr){
		vertexBuffer->SetName(L"Vertex Buffer");
	}

	//Index Buffer
	std::vector<uint32_t> dxrIndices;
	dxrIndices.reserve(numIndices);

	for(const auto& i : mesh_.indices){
		dxrIndices.push_back(i);
	}

	indexBuffer.Attach(DX12::GetInstance().CreateBuffer(dxrIndices.data(), dxrIndices.size() * sizeof(uint32_t), true, D3D12_RESOURCE_STATE_GENERIC_READ));
	GADGET_BASIC_ASSERT(indexBuffer != nullptr);
	if(indexBuffer != nullptr){
		indexBuffer->SetName(L"Index Buffer");
	}

	//Bottom Level Acceleration Structure
	bottomLevelAS = new DXR_BottomLevelAS(vertexBuffer.Get(), mesh_.vertices.size(), indexBuffer.Get(), dxrIndices.size());
}

DXR_MeshInfo::DXR_MeshInfo(size_t indexCount_, ID3D12_Resource* vertexBuffer_, ID3D12_Resource* indexBuffer_) : MeshInfo(indexCount_), vertexBuffer(nullptr), indexBuffer(nullptr), bottomLevelAS(nullptr){
	GADGET_BASIC_ASSERT(indexCount_ > 0);
	GADGET_BASIC_ASSERT(vertexBuffer_ != nullptr);
	GADGET_BASIC_ASSERT(indexBuffer_ != nullptr);

	vertexBuffer.Attach(vertexBuffer_);
	indexBuffer.Attach(indexBuffer_);
}

DXR_MeshInfo::~DXR_MeshInfo(){
	delete bottomLevelAS;
	bottomLevelAS = nullptr;
}
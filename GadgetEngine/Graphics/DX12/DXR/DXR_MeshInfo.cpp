#include "DXR_MeshInfo.h"

#include "Random.h"
#include "Graphics/Color.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_Helpers.h"
#include "Graphics/DX12/DXR/DXR.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

struct DXR_Vertex{
	Vector3 position;
	Color color;
};

DXR_MeshInfo::DXR_MeshInfo(const Mesh& mesh_) : MeshInfo(mesh_.indices.size()), vertexBuffer(nullptr), indexBuffer(nullptr), bottomLevelAS(nullptr){
	GADGET_BASIC_ASSERT(mesh_.vertices.size() > 0);
	GADGET_BASIC_ASSERT(mesh_.indices.size() > 0);

	//Vertex Buffer
	std::vector<DXR_Vertex> dxrVerts;
	dxrVerts.reserve(numIndices);

	for(const auto& i : mesh_.indices){
		dxrVerts.push_back({{ mesh_.vertices[i].position }, { Random::RandomColor() }});
	}

	vertexBuffer.Attach(DX12_Helpers::CreateBuffer(DX12::GetInstance().MainDevice(), dxrVerts.data(), dxrVerts.size() * sizeof(DXR_Vertex), true, D3D12_RESOURCE_STATE_GENERIC_READ));

	//Index Buffer
	std::vector<uint32_t> dxrIndices;
	dxrIndices.reserve(numIndices);

	for(const auto& i : mesh_.indices){
		dxrIndices.push_back(i);
	}

	indexBuffer.Attach(DX12_Helpers::CreateBuffer(DX12::GetInstance().MainDevice(), dxrIndices.data(), dxrIndices.size() * sizeof(uint32_t), true, D3D12_RESOURCE_STATE_GENERIC_READ));

	//Bottom Level Acceleration Structure
	GADGET_BASIC_ASSERT(numIndices < std::numeric_limits<uint32_t>::max());
	std::pair<ComPtr<ID3D12Resource>, uint32_t> vertBuffers = { vertexBuffer.Get(), static_cast<uint32_t>(dxrVerts.size()) };
	std::pair<ComPtr<ID3D12Resource>, uint32_t> idxBuffers = { indexBuffer.Get(), static_cast<uint32_t>(dxrIndices.size()) };
	
	AccelerationStructureBuffers bottomLevelBuffers = DXR::GetInstance().CreateBottomLevelAS({ vertBuffers }, { idxBuffers });
	bottomLevelAS.Attach(bottomLevelBuffers.pResult.Get());
}

DXR_MeshInfo::DXR_MeshInfo(size_t indexCount_, ID3D12_Resource* vertexBuffer_, ID3D12_Resource* indexBuffer_) : MeshInfo(indexCount_), vertexBuffer(nullptr), indexBuffer(nullptr), bottomLevelAS(nullptr){
	GADGET_BASIC_ASSERT(indexCount_ > 0);
	GADGET_BASIC_ASSERT(vertexBuffer_ != nullptr);
	GADGET_BASIC_ASSERT(indexBuffer_ != nullptr);

	vertexBuffer.Attach(vertexBuffer_);
	indexBuffer.Attach(indexBuffer_);
}
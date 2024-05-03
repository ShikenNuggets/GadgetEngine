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

DXR_MeshInfo::DXR_MeshInfo(const Mesh& mesh_) : MeshInfo(mesh_.indices.size()){
	//Create Vertex Buffer
	std::vector<DXR_Vertex> dxrVerts;
	dxrVerts.reserve(numIndices);

	for(const auto& i : mesh_.indices){
		dxrVerts.push_back({{ mesh_.vertices[i].position }, { Random::RandomColor() }});
	}

	vertexBuffer.Attach(DX12_Helpers::CreateBuffer(DX12::GetInstance().MainDevice(), dxrVerts.data(), dxrVerts.size() * sizeof(DXR_Vertex), true, D3D12_RESOURCE_STATE_GENERIC_READ));

	//Create Bottom Level Acceleration Structure
	GADGET_BASIC_ASSERT(numIndices < std::numeric_limits<uint32_t>::max());
	std::pair<ComPtr<ID3D12Resource>, uint32_t> buffers = { vertexBuffer.Get(), static_cast<uint32_t>(numIndices) };
	
	AccelerationStructureBuffers bottomLevelBuffers = DXR::GetInstance().CreateBottomLevelAS({ buffers });
	bottomLevelAS.Attach(bottomLevelBuffers.pResult.Get());
}

DXR_MeshInfo::DXR_MeshInfo(size_t indexCount_, ID3D12_Resource* vertexBuffer_) : MeshInfo(indexCount_), vertexBuffer(nullptr), bottomLevelAS(nullptr){
	GADGET_BASIC_ASSERT(indexCount_ > 0);
	GADGET_BASIC_ASSERT(vertexBuffer_ != nullptr);

	vertexBuffer.Attach(vertexBuffer_);
}
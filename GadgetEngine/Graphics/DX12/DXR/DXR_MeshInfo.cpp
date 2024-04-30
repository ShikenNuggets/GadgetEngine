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
	std::vector<DXR_Vertex> dxrVerts;
	dxrVerts.reserve(numIndices);

	for(const auto& i : mesh_.indices){
		dxrVerts.push_back({{ mesh_.vertices[i].position }, { Random::RandomColor() }});
	}

	vertexBuffer.Attach(DX12_Helpers::CreateBuffer(DX12::GetInstance().MainDevice(), dxrVerts.data(), dxrVerts.size() * sizeof(DXR_Vertex), true, D3D12_RESOURCE_STATE_GENERIC_READ));

	std::pair<ComPtr<ID3D12Resource>, uint32_t> buffers = { vertexBuffer.Get(), numIndices };

	AccelerationStructureBuffers bottomLevelBuffers = DXR::GetInstance().CreateBottomLevelAS({ buffers });
	bottomLevelAS.Attach(bottomLevelBuffers.pResult.Get());
}
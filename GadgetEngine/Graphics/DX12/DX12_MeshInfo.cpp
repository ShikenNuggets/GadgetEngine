#include "DX12_MeshInfo.h"

#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_Helpers.h"

using namespace Gadget;

DX12_MeshInfo::DX12_MeshInfo(const Mesh& mesh_) : MeshInfo(mesh_.indices.size()){
	std::vector<Vector3> positions;
	positions.reserve(mesh_.vertices.size());
	for(const auto& p : mesh_.vertices){
		positions.push_back(p.position);
	}
	positions.shrink_to_fit();

	GADGET_BASIC_ASSERT(positions.size() * sizeof(Vector3) < std::numeric_limits<UINT>::max());

	constexpr auto alignment = D3D12_RAW_UAV_SRV_BYTE_ALIGNMENT;
	const uint32_t alignedVertexSize = static_cast<uint32_t>(Utils::AlignSizeUp<alignment>(positions.size() * sizeof(Vector3)));
	vertexBuffer = DX12_Helpers::CreateBuffer(DX12::GetInstance().MainDevice(), positions.data(), alignedVertexSize);

	const uint32_t alignedIndexSize = static_cast<uint32_t>(Utils::AlignSizeUp<alignment>(mesh_.indices.size() * sizeof(uint32_t)));
	indexBuffer = DX12_Helpers::CreateBuffer(DX12::GetInstance().MainDevice(), positions.data(), alignedIndexSize);

	vertexBufferView = D3D12_VERTEX_BUFFER_VIEW{};
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = static_cast<UINT>(positions.size() * sizeof(Vector3));
	vertexBufferView.StrideInBytes = sizeof(Vector3);

	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = sizeof(uint32_t) * mesh_.indices.size();

	primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

DX12_MeshInfo::~DX12_MeshInfo(){
	DX12::GetInstance().DeferredRelease(indexBuffer.Detach());
	DX12::GetInstance().DeferredRelease(vertexBuffer.Detach());
}

void DX12_MeshInfo::Bind(){
	DX12::GetInstance().GfxCommand()->CommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	DX12::GetInstance().GfxCommand()->CommandList()->IASetIndexBuffer(&indexBufferView);
	DX12::GetInstance().GfxCommand()->CommandList()->IASetPrimitiveTopology(primitiveTopology);
}

void DX12_MeshInfo::Unbind(){
	//Not necessary
}
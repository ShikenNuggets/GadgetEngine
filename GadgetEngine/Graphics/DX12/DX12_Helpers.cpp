#include "DX12_Helpers.h"

#include <wrl.h>

#include "DX12.h"

using namespace Gadget;

ID3D12RootSignature* DX12_Helpers::DX12_RootSignatureDesc::Create(ID3D12Device* device_) const{
	GADGET_BASIC_ASSERT(device_ != nullptr);

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC rsDesc{};
	rsDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
	rsDesc.Desc_1_1 = *this;

	Microsoft::WRL::ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT result = D3D12SerializeVersionedRootSignature(&rsDesc, &rootSignatureBlob, &errorBlob);
	if(FAILED(result) || rootSignatureBlob == nullptr){
		const char* errorMsg = "Unspecified error";
		if(errorBlob != nullptr){
			errorMsg = (const char*)errorBlob->GetBufferPointer();
		}

		Debug::Log(SID("RENDER"), "D3D12SerializeVersionedRootSignature failed! Error Message: " + std::string(errorMsg), Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	ID3D12RootSignature* rootSignature;
	result = device_->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	if(FAILED(result) || rootSignature == nullptr){
		Debug::Log(SID("RENDER"), "ID3D12Device::CreateRootSignature failed!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	return rootSignature;
}

//----------------------------------------------------------------------------------------------------//
//-------------------- Resource Barriers -------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

void DX12_Helpers::DX12_ResourceBarriers::AddTransitionBarrier(ID3D12Resource* resource_, D3D12_RESOURCE_STATES before_, D3D12_RESOURCE_STATES after_, D3D12_RESOURCE_BARRIER_FLAGS flags_, uint32_t subResource_){
	GADGET_BASIC_ASSERT(resource_ != nullptr);

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = flags_;
	barrier.Transition.pResource = resource_;
	barrier.Transition.StateBefore = before_;
	barrier.Transition.StateAfter = after_;
	barrier.Transition.Subresource = subResource_;

	barriers.push_back(barrier);
}

void DX12_Helpers::DX12_ResourceBarriers::AddUAVBarrier(ID3D12Resource* resource_, D3D12_RESOURCE_BARRIER_FLAGS flags_){
	GADGET_BASIC_ASSERT(resource_ != nullptr);

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = flags_;
	barrier.UAV.pResource = resource_;
}

void DX12_Helpers::DX12_ResourceBarriers::AddAliasingBarrier(ID3D12Resource* beforeResource_, ID3D12Resource* afterResource_, D3D12_RESOURCE_BARRIER_FLAGS flags_){
	GADGET_BASIC_ASSERT(beforeResource_ != nullptr);
	GADGET_BASIC_ASSERT(afterResource_ != nullptr);

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
	barrier.Flags = flags_;
	barrier.Aliasing.pResourceBefore = beforeResource_;
	barrier.Aliasing.pResourceAfter = afterResource_;
}

void DX12_Helpers::DX12_ResourceBarriers::ApplyAllBarriers(ID3D12_GraphicsCommandList* cmdList_){
	GADGET_BASIC_ASSERT(cmdList_ != nullptr);

	cmdList_->ResourceBarrier(barriers.size(), barriers.data());
	barriers.clear(); //TODO - is this safe?
}
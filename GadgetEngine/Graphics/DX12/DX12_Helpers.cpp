#include "DX12_Helpers.h"

#include <wrl.h>

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
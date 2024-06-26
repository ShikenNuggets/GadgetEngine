#include "DX12_Helpers.h"

#include <wrl.h>

#include "Graphics/DX12/DX12.h"

using namespace Gadget;

ID3D12RootSignature* DX12_Helpers::DX12_RootSignatureDesc::Create(ID3D12Device* device_) const{
	GADGET_BASIC_ASSERT(device_ != nullptr);

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC rsDesc{};
	rsDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
	rsDesc.Desc_1_0 = *this;

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
	GADGET_ASSERT(barriers.size() <= std::numeric_limits<uint32_t>::max(), "D3D12 command list cannot accept more than uint32_t max barriers - tried to pass " + std::to_string(barriers.size()));

	cmdList_->ResourceBarrier(static_cast<uint32_t>(barriers.size()), barriers.data());
	barriers.clear(); //TODO - is this safe?
}

//----------------------------------------------------------------------------------------------------//
//-------------------- Buffers -----------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

ID3D12Resource* DX12_Helpers::CreateBuffer(ID3D12_Device* device_, const void* data_, uint64_t bufferSize_, bool isCpuAccessible_, D3D12_RESOURCE_STATES state_, D3D12_RESOURCE_FLAGS flags_, ID3D12Heap* heap_, uint64_t heapOffset_){
	GADGET_BASIC_ASSERT(device_ != nullptr);
	//GADGET_BASIC_ASSERT(bufferSize_ > 0);

	D3D12_RESOURCE_DESC desc{};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = Math::Clamp(static_cast<size_t>(1), std::numeric_limits<UINT64>::max(), bufferSize_);
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc = { 1, 0 };
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = isCpuAccessible_ ? D3D12_RESOURCE_FLAG_NONE : flags_;

#pragma warning(disable : 26813) //Use 'bitwise and' to check if a flag is set - == is intentional in this case
	GADGET_BASIC_ASSERT(desc.Flags == D3D12_RESOURCE_FLAG_NONE || desc.Flags == D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
#pragma warning(default : 26813)

	ID3D12Resource* resource = nullptr;
	const D3D12_RESOURCE_STATES resourceState = isCpuAccessible_ ? D3D12_RESOURCE_STATE_GENERIC_READ : state_;

	HRESULT result = S_OK;
	if(heap_ != nullptr){
		result = device_->CreatePlacedResource(heap_, heapOffset_, &desc, resourceState, nullptr, IID_PPV_ARGS(&resource));
	}else{
		const D3D12_HEAP_PROPERTIES heapProps = isCpuAccessible_ ? UploadHeapProperties : DefaultHeapProperties;
		result = device_->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, resourceState, nullptr, IID_PPV_ARGS(&resource));
	}

	if(FAILED(result) || resource == nullptr){
		GADGET_LOG_ERROR(SID("RENDER"), "Could not create resource buffer!");
		return nullptr;
	}

	if(data_){
		if(isCpuAccessible_){
			D3D12_RANGE range{ 0, 0 };
			void* cpuAddress = nullptr;
			resource->Map(0, &range, reinterpret_cast<void**>(&cpuAddress));
			GADGET_BASIC_ASSERT(cpuAddress != nullptr);
			memcpy(cpuAddress, data_, bufferSize_);
			resource->Unmap(0, nullptr);

		}else{
			DX12_UploadContext context = DX12_UploadContext(device_, bufferSize_);
			memcpy(context.CPUAddress(), data_, bufferSize_);
			context.CommandList()->CopyResource(resource, context.UploadBuffer());
			context.EndUpload();
		}
	}

	GADGET_BASIC_ASSERT(resource != nullptr);
	return resource;
}

void DX12_Helpers::UpdateBuffer(ID3D12_Resource* buffer_, const void* data_, uint64_t bufferSize_, UINT subResource_){
	if(buffer_ == nullptr){
		GADGET_LOG_WARNING(SID("RENDER"), "Tried to update a D3D12 buffer that was nullptr!");
		return;
	}

	uint8_t* pData = nullptr;
	D3D12_RANGE range{ 0, 0 };
	buffer_->Map(subResource_, &range, reinterpret_cast<void**>(&pData));
	GADGET_BASIC_ASSERT(pData != nullptr);
	memcpy(pData, data_, bufferSize_);
	buffer_->Unmap(subResource_, nullptr);
}
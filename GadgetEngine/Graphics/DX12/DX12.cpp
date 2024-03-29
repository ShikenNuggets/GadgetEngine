#include "DX12.h"

#include <wrl.h>

#include "DX12_Command.h"
#include "DX12_DescriptorHeap.h"
#include "DX12_RenderSurface.h"

using namespace Gadget;

IDXGIFactory7* DX12::dxgiFactory = nullptr;
ID3D12Device8* DX12::mainDevice = nullptr;
DX12_Command* DX12::gfxCommand = nullptr;
DX12_DescriptorHeap DX12::rtvDescriptorHeap{ D3D12_DESCRIPTOR_HEAP_TYPE_RTV };
DX12_DescriptorHeap DX12::dsvDescriptorHeap{ D3D12_DESCRIPTOR_HEAP_TYPE_DSV };
DX12_DescriptorHeap DX12::srvDescriptorHeap{ D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV };
DX12_DescriptorHeap DX12::uavDescriptorHeap{ D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV };
std::vector<IUnknown*> DX12::deferredReleases[FrameBufferCount]{};
uint32_t DX12::deferredReleaseFlag[DX12::FrameBufferCount]{};
std::mutex DX12::deferredReleaseMutex{};

bool DX12::IsInitialized(){ return mainDevice != nullptr && gfxCommand != nullptr; }

ID3D12Device8* const DX12::MainDevice(){ return mainDevice; }

uint32_t DX12::CurrentFrameIndex(){
	if(!IsInitialized()){
		return 0;
	}

	return gfxCommand->CurrentFrameIndex();
}

void DX12::CreateSwapChainForSurface(DX12_RenderSurface* surface_){
	GADGET_BASIC_ASSERT(surface_ != nullptr);
	GADGET_BASIC_ASSERT(gfxCommand != nullptr);

	surface_->CreateSwapChain(dxgiFactory, gfxCommand->CommandQueue(), DefaultRenderTargetFormat);
}

void DX12::ResizeSurface(DX12_RenderSurface* surface_, int width_, int height_){
	GADGET_BASIC_ASSERT(surface_ != nullptr);
	GADGET_BASIC_ASSERT(gfxCommand != nullptr);

	gfxCommand->Flush();
	surface_->SetSize(ScreenCoordinate(width_, height_));
}

void DX12::DeferredRelease(IUnknown* resource_){
	std::lock_guard lock{ deferredReleaseMutex };
	deferredReleases[CurrentFrameIndex()].push_back(resource_);
	SetDeferredReleaseFlag();
}

void DX12::SetDeferredReleaseFlag(){
	deferredReleaseFlag[CurrentFrameIndex()] = 1;
}

void DX12::ProcessDeferredReleases(uint32_t frameIndex_){
	std::lock_guard lock{ deferredReleaseMutex };

	deferredReleaseFlag[frameIndex_] = 0;

	rtvDescriptorHeap.ProcessDeferredFree(frameIndex_);
	dsvDescriptorHeap.ProcessDeferredFree(frameIndex_);
	srvDescriptorHeap.ProcessDeferredFree(frameIndex_);
	uavDescriptorHeap.ProcessDeferredFree(frameIndex_);

	for(const auto& resource : deferredReleases[frameIndex_]){
		resource->Release();
	}
	deferredReleases[frameIndex_].clear();
}

void DX12::ProcessAllDeferredReleases(){
	for(uint32_t i = 0; i < FrameBufferCount; i++){
		ProcessDeferredReleases(i);
	}
}

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
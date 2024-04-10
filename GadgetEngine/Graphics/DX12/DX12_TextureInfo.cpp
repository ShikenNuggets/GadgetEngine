#include "DX12_TextureInfo.h"

#include "DX12.h"
#include "DX12_Helpers.h"

using namespace Gadget;

//----------------------------------------------------------------------------------------------------//
//---------- DX12_TextureInfo ------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

DX12_TextureInfo::DX12_TextureInfo() : TextureInfo(), resource(nullptr), srvHandle(){}

DX12_TextureInfo::DX12_TextureInfo(const DX12_TextureInitInfo& info_) : TextureInfo(), resource(nullptr), srvHandle(){
	Initialize(info_);
}

DX12_TextureInfo::~DX12_TextureInfo(){
	DX12::GetInstance().DeferredRelease(resource);
}

void DX12_TextureInfo::Initialize(const DX12_TextureInitInfo& info_){
	GADGET_BASIC_ASSERT(DX12::GetInstance().IsInitialized());
	GADGET_BASIC_ASSERT(DX12::GetInstance().MainDevice() != nullptr);

	const D3D12_CLEAR_VALUE* clearValue = nullptr;
	if(info_.resourceDesc != nullptr && (info_.resourceDesc->Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET || info_.resourceDesc->Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)){
		clearValue = &info_.clearValue;
	}

	if(info_.resource != nullptr){
		resource = info_.resource;
	}else if(info_.heap != nullptr){
		GADGET_BASIC_ASSERT(info_.resourceDesc != nullptr);

		HRESULT result = DX12::GetInstance().MainDevice()->CreatePlacedResource(info_.heap, info_.allocationInfo.Offset, info_.resourceDesc, info_.initialState, clearValue, IID_PPV_ARGS(&resource));
		if(FAILED(result)){
			Debug::ThrowFatalError(SID("RENDER"), "ID3D12Device::CreatePlacedResource failed!", __FILE__, __LINE__);
		}
	}else{
		GADGET_BASIC_ASSERT(info_.resourceDesc != nullptr);

		HRESULT result = DX12::GetInstance().MainDevice()->CreateCommittedResource(&DX12_Helpers::DefaultHeapProperties, D3D12_HEAP_FLAG_NONE, info_.resourceDesc, info_.initialState, clearValue, IID_PPV_ARGS(&resource));
		if(FAILED(result)){
			Debug::ThrowFatalError(SID("RENDER"), "ID3D12Device::CreateCommittedResource failed!", __FILE__, __LINE__);
		}
	}

	GADGET_BASIC_ASSERT(resource != nullptr);
	srvHandle = DX12::GetInstance().SRVHeap().Allocate();
	DX12::GetInstance().MainDevice()->CreateShaderResourceView(resource, info_.srvDesc, srvHandle.cpuHandle);
}

//----------------------------------------------------------------------------------------------------//
//---------- DX12_RenderTextureInfo ------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

DX12_RenderTextureInfo::DX12_RenderTextureInfo() : DX12_TextureInfo(), rtvHandles(), mipCount(0){}

DX12_RenderTextureInfo::DX12_RenderTextureInfo(const DX12_TextureInitInfo& info_) : DX12_TextureInfo(info_), rtvHandles(), mipCount(0){
	mipCount = resource->GetDesc().MipLevels;
	GADGET_BASIC_ASSERT(mipCount != 0);
	GADGET_BASIC_ASSERT(mipCount < maxMips);

	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format = info_.resourceDesc->Format;
	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	for(; desc.Texture2D.MipSlice < mipCount; desc.Texture2D.MipSlice++){
		rtvHandles[desc.Texture2D.MipSlice] = DX12::GetInstance().RTVHeap().Allocate();
		DX12::GetInstance().MainDevice()->CreateRenderTargetView(resource, &desc, rtvHandles[desc.Texture2D.MipSlice].cpuHandle);
	}
}

DX12_RenderTextureInfo::~DX12_RenderTextureInfo(){
	for(uint32_t i = 0; i < mipCount; i++){
		DX12::GetInstance().RTVHeap().Free(rtvHandles[i]);
	}
	mipCount = 0;
}

D3D12_CPU_DESCRIPTOR_HANDLE DX12_RenderTextureInfo::RTV(uint32_t mipIndex_) const{
	GADGET_BASIC_ASSERT(mipIndex_ < mipCount);
	if(mipIndex_ >= mipCount){
		Debug::Log(SID("RENDER"), "Tried to get D3D12_CPU_DESCRIPTOR_HANDLE for invalid mip index " + std::to_string(mipIndex_) + "!");
		return D3D12_CPU_DESCRIPTOR_HANDLE{};
	}

	return rtvHandles[mipIndex_].cpuHandle;
}

//----------------------------------------------------------------------------------------------------//
//---------- DX12_DepthBufferTextureInfo -------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

DX12_DepthBufferTextureInfo::DX12_DepthBufferTextureInfo() : DX12_TextureInfo(), dsvHandle(){}

DX12_DepthBufferTextureInfo::DX12_DepthBufferTextureInfo(DX12_TextureInitInfo info_) : dsvHandle(){
	const DXGI_FORMAT dsvFormat = info_.resourceDesc->Format;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	if(info_.resourceDesc->Format == DXGI_FORMAT_D32_FLOAT){
		info_.resourceDesc->Format = DXGI_FORMAT_R32_TYPELESS;
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	}

	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.PlaneSlice = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	GADGET_BASIC_ASSERT(info_.srvDesc == nullptr);
	GADGET_BASIC_ASSERT(info_.resource == nullptr);
	info_.srvDesc = &srvDesc;

	Initialize(info_);

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.Format = dsvFormat;
	dsvDesc.Texture2D.MipSlice = 0;

	dsvHandle = DX12::GetInstance().DSVHeap().Allocate();
	DX12::GetInstance().MainDevice()->CreateDepthStencilView(resource, &dsvDesc, dsvHandle.cpuHandle);
}

DX12_DepthBufferTextureInfo::~DX12_DepthBufferTextureInfo(){
	DX12::GetInstance().DSVHeap().Free(dsvHandle);
}
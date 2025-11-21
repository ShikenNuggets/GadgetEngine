#include "DXR_OutputResource.h"

#include "Debug.h"
#include "Graphics/DX12/DX12.h"

using namespace Gadget;

DXR_OutputResource::DXR_OutputResource(const ScreenCoordinate& frameSize_) : outputResource(nullptr){
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());
	GADGET_BASIC_ASSERT(frameSize_.x > 0);
	GADGET_BASIC_ASSERT(frameSize_.y > 0);

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.DepthOrArraySize = 1;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	resDesc.Width = frameSize_.x;
	resDesc.Height = frameSize_.y;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;

	HRESULT hr = DX12::GetInstance().MainDevice()->CreateCommittedResource(&DX12_Helpers::DefaultHeapProperties, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_COPY_SOURCE, nullptr, IID_PPV_ARGS(outputResource.ReleaseAndGetAddressOf()));
	if(FAILED(hr) || outputResource == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create output resource!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}

	outputResource->SetName(L"DXR Output Resource");
}
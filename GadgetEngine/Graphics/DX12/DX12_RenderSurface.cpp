#include "DX12_RenderSurface.h"

#include "Platform/Windows/Win32_Window.h"

using namespace Gadget;

DX12_RenderSurface::DX12_RenderSurface(Window* parent_, int w_, int h_) : RenderSurface(parent_, w_, h_), swapChain(nullptr), renderTargetData(), viewPort(), scissorRect(){}

DX12_RenderSurface::~DX12_RenderSurface(){
	Release();
}

void DX12_RenderSurface::CreateSwapChain(IDXGIFactory7* factory_, ID3D12CommandQueue* cmdQueue, DXGI_FORMAT format_){
	if(format_ == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB){
		format_ = DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	GADGET_BASIC_ASSERT(factory_ != nullptr);
	GADGET_BASIC_ASSERT(cmdQueue != nullptr);
	static_assert(DX12::FrameBufferCount >= 2, "DX12 swap chains require at least two framebuffers");

	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = DX12::FrameBufferCount;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags = 0;
	desc.Format = format_;
	desc.Width = size.x;
	desc.Height = size.y;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Stereo = false;

	IDXGISwapChain1* tempSwapChain = nullptr;
	HWND hwnd = (HWND)parent->GetWindowHandle();
	HRESULT hr = factory_->CreateSwapChainForHwnd(cmdQueue, hwnd, &desc, nullptr, nullptr, &tempSwapChain);
	if(FAILED(hr) || tempSwapChain == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create swap chain for HWND [" + std::to_string((uint64_t)hwnd) + "]", __FILE__, __LINE__);
	}

	hr = factory_->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
	if(FAILED(hr)){
		Debug::ThrowFatalError(SID("RENDER"), "Could not make window association to HWND [" + std::to_string((uint64_t)hwnd) + "]", __FILE__, __LINE__);
	}

	hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain));
	if(FAILED(hr)){
		Debug::ThrowFatalError(SID("RENDER"), "IDXGISwapChain1 could not be queried as IDXGISwapChain4!", __FILE__, __LINE__);
	}

	tempSwapChain->Release();

	for(uint32_t i = 0; i < DX12::FrameBufferCount; i++){
		renderTargetData[i].renderTargetView = DX12::RTVHeap().Allocate();
	}

	FinalizeSwapChain();
}

void DX12_RenderSurface::Present() const{
	GADGET_BASIC_ASSERT(swapChain != nullptr);

	if(swapChain != nullptr){
		swapChain->Present(0, 0);
	}
}

void DX12_RenderSurface::SetSize([[maybe_unused]] const ScreenCoordinate& sc_){
	//TODO
}

void DX12_RenderSurface::Release(){
	for(uint32_t i = 0; i < DX12::FrameBufferCount; i++){
		auto& data = renderTargetData[i];
		if(data.resource != nullptr){
			data.resource->Release();
		}
		DX12::RTVHeap().Free(data.renderTargetView);
	}

	if(swapChain != nullptr){
		swapChain->Release();
	}
}

void DX12_RenderSurface::FinalizeSwapChain(){
	for(uint32_t i = 0; i < DX12::FrameBufferCount; i++){
		DX12_RenderTargetData& data = renderTargetData[i];
		//GADGET_BASIC_ASSERT(data.resource != nullptr);

		HRESULT hr = swapChain->GetBuffer(i, IID_PPV_ARGS(&data.resource));
		if(FAILED(hr)){
			Debug::ThrowFatalError(SID("RENDER"), "Could not get buffer from swap chain!", __FILE__, __LINE__);
		}

		D3D12_RENDER_TARGET_VIEW_DESC desc{};
		desc.Format = DX12::DefaultRenderTargetFormat;
		desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		
		DX12::MainDevice()->CreateRenderTargetView(data.resource, &desc, data.renderTargetView.cpuHandle);
	}

	DXGI_SWAP_CHAIN_DESC desc;
	HRESULT hr = swapChain->GetDesc(&desc);
	if(FAILED(hr)){
		Debug::ThrowFatalError(SID("RENDER"), "Could not get description from swap chain!", __FILE__, __LINE__);
	}

	GADGET_BASIC_ASSERT((unsigned int)size.x == desc.BufferDesc.Width);
	GADGET_BASIC_ASSERT((unsigned int)size.y == desc.BufferDesc.Height);

	size.x = (int)desc.BufferDesc.Width;
	size.y = (int)desc.BufferDesc.Height;

	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = (float)size.x;
	viewPort.Height = (float)size.y;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	scissorRect = { 0, 0, size.x, size.y };
}
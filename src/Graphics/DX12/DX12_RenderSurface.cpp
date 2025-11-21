#include "DX12_RenderSurface.h"

#include "App.h"
#include "Graphics/DX12/DX12.h"
#include "Platform/Windows/Win32_Window.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

DX12_RenderSurface::DX12_RenderSurface(Window* parent_, int w_, int h_) : RenderSurface(parent_, w_, h_), swapChain(nullptr), renderTargetData(DX12::FrameBufferCount), allowTearing(0), presentFlags(0), viewPort(), scissorRect(), format(DefaultBackBufferFormat){}

DX12_RenderSurface::~DX12_RenderSurface(){
	Release();
}

ErrorCode DX12_RenderSurface::CreateSwapChain(IDXGI_Factory* factory_, ID3D12CommandQueue* cmdQueue, DXGI_FORMAT format_){
	static_assert(DX12::FrameBufferCount >= 2, "DX12 swap chains require at least two framebuffers");
	GADGET_BASIC_ASSERT(factory_ != nullptr);
	GADGET_BASIC_ASSERT(cmdQueue != nullptr);

	if(factory_ == nullptr || cmdQueue == nullptr){
		GADGET_LOG_WARNING(SID("RENDER"), "Tried to create swapchain with null parameters");
		return ErrorCode::Invalid_Args;
	}

	format = format_;
	if(format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB){
		format = DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	HRESULT hr = factory_->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(uint32_t));
	if(SUCCEEDED(hr) && allowTearing == TRUE && !App::GetConfig().GetOptionBool(EngineVars::Display::vsyncKey)){
		presentFlags = DXGI_PRESENT_ALLOW_TEARING;
	}else{
		presentFlags = 0;
	}

	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = DX12::FrameBufferCount;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags = allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
	desc.Format = format;
	desc.Width = size.x;
	desc.Height = size.y;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Stereo = false;

	ComPtr<IDXGISwapChain1> tempSwapChain = nullptr;
	HWND hwnd = reinterpret_cast<HWND>(parent->GetWindowHandle());
	hr = factory_->CreateSwapChainForHwnd(cmdQueue, hwnd, &desc, nullptr, nullptr, &tempSwapChain);
	if(FAILED(hr) || tempSwapChain == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create swap chain for HWND [" + std::to_string((uint64_t)hwnd) + "]", ErrorCode::D3D12_Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	hr = factory_->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
	if(FAILED(hr)){
		Debug::ThrowFatalError(SID("RENDER"), "Could not make window association to HWND [" + std::to_string((uint64_t)hwnd) + "]", ErrorCode::D3D12_Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	hr = tempSwapChain.As(&swapChain);
	if(FAILED(hr)){
		Debug::ThrowFatalError(SID("RENDER"), "IDXGISwapChain1 could not be queried as IDXGISwapChain3", ErrorCode::D3D12_Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	for(uint32_t i = 0; i < DX12::FrameBufferCount; i++){
		renderTargetData[i].renderTargetView = DX12::GetInstance().RTVHeap().Allocate();
	}

	return FinalizeSwapChain();
}

ErrorCode DX12_RenderSurface::Present() const{
	GADGET_BASIC_ASSERT(swapChain != nullptr);
	if(swapChain == nullptr){
		return ErrorCode::Invalid_State;
	}

	if(FAILED(swapChain->Present(0, presentFlags))){
		GADGET_LOG_ERROR(SID("RENDER"), "Could not present the swapchain");
		return ErrorCode::D3D12_Error;
	}

	return ErrorCode::OK;
}

ErrorCode DX12_RenderSurface::SetSize(const ScreenCoordinate& sc_){
	if(sc_.x == size.x && sc_.y == size.y){
		return ErrorCode::OK; //Size didn't change, no need to do anything
	}

	GADGET_BASIC_ASSERT(swapChain != nullptr);
	if(swapChain == nullptr){
		GADGET_LOG_WARNING(SID("RENDER"), "Set Size called on an uninitialized DX12_RenderSurface");
		return ErrorCode::Invalid_State;
	}

	size = sc_;

	for(uint32_t i = 0; i < DX12::FrameBufferCount; i++){
		renderTargetData[i].resource.Reset();
	}

	const uint32_t flags = 0;
	swapChain->ResizeBuffers(DX12::FrameBufferCount, 0, 0, DXGI_FORMAT_UNKNOWN, flags);

	auto err = FinalizeSwapChain();
	if(err != ErrorCode::OK){
		GADGET_LOG_ERROR(SID("RENDER"), "Could not finalize swapchain during resize operation");
		return err;
	}

	GADGET_LOG(SID("RENDER"), "DX12_RenderSurface was resized to " + std::to_string(size.x) + "x" + std::to_string(size.y));

	return ErrorCode::OK;
}

void DX12_RenderSurface::Release(){
	for(uint32_t i = 0; i < DX12::FrameBufferCount; i++){
		renderTargetData[i].resource.Reset();
		DX12::GetInstance().RTVHeap().Free(renderTargetData[i].renderTargetView);
	}

	swapChain.Reset();
}

ErrorCode DX12_RenderSurface::FinalizeSwapChain(){
	for(uint32_t i = 0; i < DX12::FrameBufferCount; i++){
		DX12_RenderTargetData& data = renderTargetData[i];

		HRESULT hr = swapChain->GetBuffer(i, IID_PPV_ARGS(data.resource.ReleaseAndGetAddressOf()));
		if(FAILED(hr)){
			GADGET_LOG_ERROR(SID("RENDER"), "Could not get buffer from swap chain!");
			return ErrorCode::D3D12_Error;
		}

		D3D12_RENDER_TARGET_VIEW_DESC desc{};
		desc.Format = format;
		desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		
		DX12::GetInstance().MainDevice()->CreateRenderTargetView(data.resource.Get(), &desc, data.renderTargetView.cpuHandle);
	}

	DXGI_SWAP_CHAIN_DESC desc;
	HRESULT hr = swapChain->GetDesc(&desc);
	if(FAILED(hr)){
		GADGET_LOG_ERROR(SID("RENDER"), "Could not get description from swap chain!");
		return ErrorCode::D3D12_Error;
	}

	GADGET_BASIC_ASSERT((unsigned int)size.x == desc.BufferDesc.Width); //Updating these values should have already be handled
	GADGET_BASIC_ASSERT((unsigned int)size.y == desc.BufferDesc.Height); //We'll take care of it here either way, but if these asserts fail then something might have gone wrong

	size.x = (int)desc.BufferDesc.Width;
	size.y = (int)desc.BufferDesc.Height;

	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = (float)size.x;
	viewPort.Height = (float)size.y;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	scissorRect = { 0, 0, size.x, size.y };

	return ErrorCode::OK;
}
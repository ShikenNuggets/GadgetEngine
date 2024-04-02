#ifndef GADGET_DX12_RENDER_SURFACE_H
#define GADGET_DX12_RENDER_SURFACE_H

#include "RenderSurface.h"
#include "DX12.h"
#include "DX12_DescriptorHeap.h"

namespace Gadget{
	struct DX12_RenderTargetData{
		ID3D12Resource* resource = nullptr;
		DX12_DescriptorHandle renderTargetView{};
	};

	class DX12_RenderSurface : public RenderSurface{
	public:
		static constexpr DXGI_FORMAT DefaultBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		explicit DX12_RenderSurface(Window* parent_, int w_, int h_);
		virtual ~DX12_RenderSurface() override;

		constexpr ID3D12Resource* const CurrentBackBuffer() const{ return renderTargetData[swapChain->GetCurrentBackBufferIndex()].resource; }
		constexpr D3D12_CPU_DESCRIPTOR_HANDLE CurrentRenderTargetView() const{ return renderTargetData[swapChain->GetCurrentBackBufferIndex()].renderTargetView.cpuHandle; }
		constexpr const D3D12_VIEWPORT& Viewport() const{ return viewPort; }
		constexpr const D3D12_RECT& ScissorRect() const{ return scissorRect; }

		void CreateSwapChain(IDXGIFactory7* factory_, ID3D12CommandQueue* cmdQueue, DXGI_FORMAT format_ = DefaultBackBufferFormat);
		void Present() const;
		virtual void SetSize(const ScreenCoordinate& sc_) override;

	private:
		IDXGISwapChain4* swapChain;
		DX12_RenderTargetData renderTargetData[DX12::FrameBufferCount];
		uint32_t allowTearing;
		uint32_t presentFlags;
		D3D12_VIEWPORT viewPort;
		D3D12_RECT scissorRect;
		DXGI_FORMAT format;

		void Release();
		void FinalizeSwapChain();
	};
}

#endif //!GADGET_DX12_RENDER_SURFACE_H
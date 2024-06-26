#ifndef GADGET_DX12_RENDER_SURFACE_H
#define GADGET_DX12_RENDER_SURFACE_H

#include <wrl/client.h>

#include "GadgetEnums.h"
#include "RenderSurface.h"
#include "Graphics/DX12/DX12_DescriptorHeap.h"

namespace Gadget{
	struct DX12_RenderTargetData{
		Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
		DX12_DescriptorHandle renderTargetView{};
	};

	class DX12_RenderSurface : public RenderSurface{
	public:
		static constexpr DXGI_FORMAT DefaultBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		explicit DX12_RenderSurface(Window* parent_, int w_, int h_);
		virtual ~DX12_RenderSurface() override;

		UINT CurrentBackBufferIndex() const{ return swapChain->GetCurrentBackBufferIndex(); }
		ID3D12Resource* CurrentBackBuffer() const{ return renderTargetData[CurrentBackBufferIndex()].resource.Get(); }
		constexpr D3D12_CPU_DESCRIPTOR_HANDLE CurrentRenderTargetView() const{ return renderTargetData[swapChain->GetCurrentBackBufferIndex()].renderTargetView.cpuHandle; }
		constexpr const D3D12_VIEWPORT& Viewport() const{ return viewPort; }
		constexpr const D3D12_RECT& ScissorRect() const{ return scissorRect; }

		[[nodiscard]] ErrorCode CreateSwapChain(IDXGI_Factory* factory_, ID3D12CommandQueue* cmdQueue, DXGI_FORMAT format_ = DefaultBackBufferFormat);
		[[nodiscard]] ErrorCode Present() const;
		[[nodiscard]] virtual ErrorCode SetSize(const ScreenCoordinate& sc_) override;

	private:
		Microsoft::WRL::ComPtr<IDXGI_SwapChain> swapChain;
		std::vector<DX12_RenderTargetData> renderTargetData;
		uint32_t allowTearing;
		uint32_t presentFlags;
		D3D12_VIEWPORT viewPort;
		D3D12_RECT scissorRect;
		DXGI_FORMAT format;

		void Release();
		[[nodiscard]] ErrorCode FinalizeSwapChain();
	};
}

#endif //!GADGET_DX12_RENDER_SURFACE_H
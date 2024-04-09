#ifndef GADGET_DX12_H
#define GADGET_DX12_H

#include <mutex>

#include <wrl/client.h>

#include "DX12_Defines.h"
#include "DX12_Helpers.h"
#include "Debug.h"
#include "GadgetEnums.h"
#include "Utils/Utils.h"

namespace Gadget{
	class DX12_Command;
	class DX12_DescriptorHeap;
	class DX12_RenderSurface;

	class DX12{
	public:
		DISABLE_COPY_AND_MOVE(DX12)

		DX12(){ GADGET_BASIC_ASSERT(!IsInitialized()); }

		[[nodiscard]] static ErrorCode EnableDebugLayer(bool gpuValidation_ = false);
		[[nodiscard]] static ErrorCode CreateDevice(uint32_t dxgiFactoryFlags_ = 0);
		[[nodiscard]] static ErrorCode CreateCommandList();
		[[nodiscard]] static ErrorCode InitializeDescriptorHeaps();
		[[nodiscard]] static ErrorCode BreakOnWarningsAndErrors(bool enabled_);

		[[nodiscard]] static ErrorCode PreShutdown();
		[[nodiscard]] static ErrorCode Shutdown();

		static bool IsInitialized();
		static ID3D12_Device* const MainDevice();
		static DX12_Command* const GfxCommand(){ return gfxCommand; }
		static uint32_t CurrentFrameIndex();

		static void DeferredRelease(IUnknown* resource_);
		static uint32_t GetDeferredReleaseFlag();
		static void SetDeferredReleaseFlag();
		static void ProcessDeferredReleases(uint32_t frameIndex_);
		static void ProcessAllDeferredReleases(); //ONLY USE THIS ON SHUTDOWN

		static DX12_DescriptorHeap& RTVHeap(){ return rtvDescriptorHeap; }
		static DX12_DescriptorHeap& DSVHeap(){ return dsvDescriptorHeap; }
		static DX12_DescriptorHeap& SRVHeap(){ return srvDescriptorHeap; }
		static DX12_DescriptorHeap& UAVHeap(){ return uavDescriptorHeap; }
		static DX12_Helpers::DX12_ResourceBarriers& ResourceBarriers(){ return resourceBarriers; }

		static void CreateSwapChainForSurface(DX12_RenderSurface* surface_);
		static void ResizeSurface(DX12_RenderSurface* surface_, int width_, int height_);

		static constexpr uint32_t FrameBufferCount = 3;

	protected:
		static bool debugLayerEnabled;
		static Microsoft::WRL::ComPtr<IDXGI_Factory> dxgiFactory;
		static Microsoft::WRL::ComPtr<ID3D12_Device> mainDevice;
		static DX12_Command* gfxCommand;
		static DX12_Helpers::DX12_ResourceBarriers resourceBarriers;
		static DX12_DescriptorHeap rtvDescriptorHeap;
		static DX12_DescriptorHeap dsvDescriptorHeap;
		static DX12_DescriptorHeap srvDescriptorHeap;
		static DX12_DescriptorHeap uavDescriptorHeap;
		static std::vector<IUnknown*> deferredReleases[FrameBufferCount];
		static uint32_t deferredReleaseFlag[FrameBufferCount];
		static std::mutex deferredReleaseMutex;

		[[nodiscard]] static ErrorCode DebugShutdown();

		static IDXGI_Adapter* DetermineMainAdapter();
		static D3D_FEATURE_LEVEL GetMaxFeatureLevel(IDXGI_Adapter* adapter_);
	};
}

#endif //!GADGET_DX12_H
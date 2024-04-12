#ifndef GADGET_DX12_H
#define GADGET_DX12_H

#include <mutex>

#include <wrl/client.h>

#include "Debug.h"
#include "GadgetEnums.h"
#include "Graphics/DX12/DX12_Defines.h"
#include "Graphics/DX12/DX12_DescriptorHeap.h"
#include "Graphics/DX12/DX12_Helpers.h"
#include "Utils/Utils.h"

namespace Gadget{
	class DX12_Command;
	class DX12_RenderSurface;

	struct DX12_StartupOptions{
		bool isDebug = true;
		bool gpuBasedValidation = false;
		uint32_t dxgiFactoryFlags = 0;
	};

	struct DX12_DeferredRelease{
		uint32_t flag = 0;
		std::vector<IUnknown*> resources;
	};

	//Singleton responsible for managing the global DirectX 12 state
	//High level renderer should generally avoid directly calling D3D12 functions and go through this instead
	class DX12{
	public:
		DX12(const DX12_StartupOptions& options_);
		~DX12();
		DISABLE_COPY_AND_MOVE(DX12);

		static DX12& GetInstance(const DX12_StartupOptions& options_ = DX12_StartupOptions());
		static ErrorCode DeleteInstance();

		[[nodiscard]] ErrorCode EnableDebugLayer(bool gpuValidation_ = false);
		[[nodiscard]] ErrorCode CreateDevice(uint32_t dxgiFactoryFlags_ = 0);
		[[nodiscard]] ErrorCode CreateCommandList();
		[[nodiscard]] ErrorCode InitializeDescriptorHeaps();
		[[nodiscard]] ErrorCode BreakOnWarningsAndErrors(bool enabled_);

		[[nodiscard]] ErrorCode PreShutdown();
		[[nodiscard]] ErrorCode Shutdown();

		bool IsInitialized() const;
		ID3D12_Device* const MainDevice() const;
		DX12_Command* const GfxCommand() const{ return gfxCommand; }
		uint32_t CurrentFrameIndex() const;

		void DeferredRelease(IUnknown* resource_);
		uint32_t GetDeferredReleaseFlag();
		void SetDeferredReleaseFlag();
		void ProcessDeferredReleases(uint32_t frameIndex_);
		void ProcessAllDeferredReleases(); //ONLY USE THIS ON SHUTDOWN

		DX12_DescriptorHeap& RTVHeap(){ return rtvDescriptorHeap; }
		DX12_DescriptorHeap& DSVHeap(){ return dsvDescriptorHeap; }
		DX12_DescriptorHeap& SRVHeap(){ return srvDescriptorHeap; }
		DX12_DescriptorHeap& UAVHeap(){ return uavDescriptorHeap; }
		DX12_Helpers::DX12_ResourceBarriers& ResourceBarriers(){ return resourceBarriers; }

		[[nodiscard]] ErrorCode CreateSwapChainForSurface(DX12_RenderSurface* surface_);
		[[nodiscard]] ErrorCode ResizeSurface(DX12_RenderSurface* surface_, int width_, int height_);

		static constexpr uint32_t FrameBufferCount = 3;

	private:
		static std::unique_ptr<DX12> instance;

		Microsoft::WRL::ComPtr<IDXGI_Factory> dxgiFactory;
		Microsoft::WRL::ComPtr<ID3D12_Device> mainDevice;
		DX12_Command* gfxCommand;
		DX12_Helpers::DX12_ResourceBarriers resourceBarriers;
		DX12_DescriptorHeap rtvDescriptorHeap;
		DX12_DescriptorHeap dsvDescriptorHeap;
		DX12_DescriptorHeap srvDescriptorHeap;
		DX12_DescriptorHeap uavDescriptorHeap;
		std::vector<DX12_DeferredRelease> deferredReleases;
		std::mutex deferredReleaseMutex;

		[[nodiscard]] ErrorCode DebugShutdown();

		IDXGI_Adapter* DetermineMainAdapter();
		D3D_FEATURE_LEVEL GetMaxFeatureLevel(IDXGI_Adapter* adapter_);

		bool DoesDeviceSupportRaytracing(ID3D12_Device* device_) const;
	};
}

#endif //!GADGET_DX12_H
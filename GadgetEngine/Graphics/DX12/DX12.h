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
		bool breakOnWarnings = false;
		bool breakOnErrors = false;
		bool gpuBasedValidation = false;
		bool requireDXR = false;
		bool closeCommandListOnInit = true;
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
		[[nodiscard]] static ErrorCode DeleteInstance();
		static bool IsInstanceInitialized();

		[[nodiscard]] ErrorCode EnableDebugLayer(bool gpuValidation_ = false);
		[[nodiscard]] ErrorCode CreateDevice(uint32_t dxgiFactoryFlags_ = 0, bool requireDXR_ = false);
		[[nodiscard]] ErrorCode RegisterDebugCallback();
		[[nodiscard]] ErrorCode CreateCommandList(bool closeCommandListOnInit_ = true);
		[[nodiscard]] ErrorCode InitializeDescriptorHeaps();
		[[nodiscard]] ErrorCode BreakOnWarningsAndErrors(bool breakOnWarnings_, bool breakOnErrors_);

		[[nodiscard]] ErrorCode PreShutdown();
		[[nodiscard]] ErrorCode Shutdown();

		bool IsInitialized() const;
		ID3D12_Device* MainDevice() const;
		DX12_Command* GfxCommand() const{ return gfxCommand; }
		uint32_t CurrentFrameIndex() const;

		[[nodiscard]] ErrorCode FlushAndSetFrameIndex(uint32_t frameIndex_); //Calling this in the main render loop is not recommended, it should only be used on startup

		ID3D12_Resource* CreateBuffer(const void* data_, uint64_t bufferSize_, bool isCpuAccessible_ = false,
			D3D12_RESOURCE_STATES state_ = D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_FLAGS flags_ = D3D12_RESOURCE_FLAG_NONE,
			ID3D12Heap* heap_ = nullptr, uint64_t heapOffset_ = 0);

		void DeferredRelease(IUnknown* resource_);
		uint32_t GetDeferredReleaseFlag();
		void SetDeferredReleaseFlag();
		void ProcessDeferredReleases(uint32_t frameIndex_);
		void ProcessDeferredReleasesForCurrentFrame();
		void ProcessAllDeferredReleases(); //ONLY USE THIS ON SHUTDOWN

		DX12_DescriptorHeap& RTVHeap(){ return rtvDescriptorHeap; }
		DX12_DescriptorHeap& DSVHeap(){ return dsvDescriptorHeap; }
		DX12_DescriptorHeap& SRVHeap(){ return srvDescriptorHeap; }
		DX12_DescriptorHeap& UAVHeap(){ return uavDescriptorHeap; }

		DX12_DescriptorHandle CreateUAV(ID3D12_Resource* resource_, ID3D12_Resource* counterResource_, const D3D12_UNORDERED_ACCESS_VIEW_DESC* desc_, DX12_DescriptorHeap& heap_ = DX12::GetInstance().SRVHeap());
		DX12_DescriptorHandle CreateSRV(ID3D12_Resource* resource_, const D3D12_SHADER_RESOURCE_VIEW_DESC* desc_, DX12_DescriptorHeap& heap_ = DX12::GetInstance().SRVHeap());
		DX12_DescriptorHandle CreateCBV(const D3D12_CONSTANT_BUFFER_VIEW_DESC* desc_, DX12_DescriptorHeap& heap_ = DX12::GetInstance().SRVHeap());

		DX12_Helpers::DX12_ResourceBarriers& ResourceBarriers(){ return resourceBarriers; }

		[[nodiscard]] ErrorCode CreateSwapChainForSurface(DX12_RenderSurface* surface_);
		[[nodiscard]] ErrorCode ResizeSurface(DX12_RenderSurface* surface_, int width_, int height_);

		static constexpr uint32_t FrameBufferCount = 3;

	private:
		static std::unique_ptr<DX12> instance;

		D3D_FEATURE_LEVEL minimumFeatureLevel;
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
		[[nodiscard]] ErrorCode UnregisterDebugCallback();

		IDXGI_Adapter* DetermineMainAdapter(bool requireDXR_);
		D3D_FEATURE_LEVEL GetMaxFeatureLevel(IDXGI_Adapter* adapter_);

		bool DoesDeviceSupportRaytracing(ID3D12_Device* device_) const;

		static DWORD callbackCookie;
		static void DebugMessageCallback(D3D12_MESSAGE_CATEGORY category_, D3D12_MESSAGE_SEVERITY severity_, D3D12_MESSAGE_ID id_, LPCSTR pDescription_, void* pContext_);
	};
}

#endif //!GADGET_DX12_H
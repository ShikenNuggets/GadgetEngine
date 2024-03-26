#ifndef GADGET_DX12_H
#define GADGET_DX12_H

#include <mutex>

#include <dxgi1_6.h>
#include <d3d12.h>

#include "Utils/Utils.h"

namespace Gadget{
	class DX12_Command;
	class DX12_DescriptorHeap;

	class DX12{
	public:
		DISABLE_COPY_AND_MOVE(DX12)
		DX12(){}

		static bool IsInitialized();
		static ID3D12Device8* const MainDevice();
		static uint32_t CurrentFrameIndex();

		static void DeferredRelease(IUnknown* resource_);
		static void SetDeferredReleaseFlag();
		static void ProcessDeferredReleases(uint32_t frameIndex_);

		static DX12_DescriptorHeap& RTVHeap(){ return rtvDescriptorHeap; }
		static DX12_DescriptorHeap& DSVHeap(){ return dsvDescriptorHeap; }
		static DX12_DescriptorHeap& SRVHeap(){ return srvDescriptorHeap; }
		static DX12_DescriptorHeap& UAVHeap(){ return uavDescriptorHeap; }

		static constexpr uint32_t FrameBufferCount = 3;
		static constexpr DXGI_FORMAT DefaultRenderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	protected:
		static ID3D12Device8* mainDevice;
		static DX12_Command* gfxCommand;
		static DX12_DescriptorHeap rtvDescriptorHeap;
		static DX12_DescriptorHeap dsvDescriptorHeap;
		static DX12_DescriptorHeap srvDescriptorHeap;
		static DX12_DescriptorHeap uavDescriptorHeap;
		static std::vector<IUnknown*> deferredReleases[FrameBufferCount];
		static uint32_t deferredReleaseFlag[FrameBufferCount];
		static std::mutex deferredReleaseMutex;
	};
}

#endif //!GADGET_DX12_H
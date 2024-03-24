#ifndef GADGET_DX12_H
#define GADGET_DX12_H

#include <mutex>

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

		static constexpr uint32_t FrameBufferCount = 3;

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
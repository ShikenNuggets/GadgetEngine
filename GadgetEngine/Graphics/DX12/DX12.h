#ifndef GADGET_DX12_H
#define GADGET_DX12_H

#include <d3d12.h>

#include "Utils/Utils.h"

namespace Gadget{
	class DX12_Command;

	class DX12{
	public:
		DISABLE_COPY_AND_MOVE(DX12)
		DX12(){}

		static bool IsInitialized();
		static ID3D12Device8* const MainDevice();
		static uint32_t CurrentFrameIndex();

		static void SetDeferredReleaseFlag();

		static constexpr uint32_t FrameBufferCount = 3;

	protected:
		static ID3D12Device8* mainDevice;
		static DX12_Command* gfxCommand;
		static uint32_t deferredReleaseFlag[FrameBufferCount];
	};
}

#endif //!GADGET_DX12_H
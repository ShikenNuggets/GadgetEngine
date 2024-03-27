#ifndef GADGET_DX12_COMMAND_H
#define GADGET_DX12_COMMAND_H

#include <d3d12.h>

#include "Debug.h"
#include "Graphics/DX12/DX12.h"
#include "Utils/Utils.h"

namespace Gadget{
	struct DX12_CommandFrame{
		DISABLE_COPY_AND_MOVE(DX12_CommandFrame)

		ID3D12CommandAllocator* cmdAllocator;
		uint64_t fenceValue;

		DX12_CommandFrame();

		void Wait(HANDLE fenceEvent_, ID3D12Fence1* fence_) const;

		void Release();
	};

	class DX12_Command{
	public:
		//Disable copy and move since this deals with raw DirectX pointers and objects
		DISABLE_COPY_AND_MOVE(DX12_Command)

		explicit DX12_Command(ID3D12Device8* const device_, D3D12_COMMAND_LIST_TYPE type_);
		~DX12_Command();

		void BeginFrame();
		void EndFrame();
		void Flush();

		constexpr ID3D12CommandQueue* const CommandQueue() const{ return cmdQueue; }
		constexpr ID3D12GraphicsCommandList6* const CommandList() const{ return cmdList; }
		constexpr uint32_t CurrentFrameIndex() const{ return frameIndex; }

	private:
		ID3D12CommandQueue* cmdQueue;
		ID3D12GraphicsCommandList6* cmdList;
		ID3D12Fence1* fence;
		uint64_t fenceValue;
		HANDLE fenceEvent;
		DX12_CommandFrame cmdFrames[DX12::FrameBufferCount];
		uint32_t frameIndex;

		void Release();
	};
}

#endif //!GADGET_DX12_COMMAND_H
#ifndef GADGET_DX12_COMMAND_H
#define GADGET_DX12_COMMAND_H

#include <d3d12.h>
#include <wrl/client.h>

#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_RenderSurface.h"
#include "Utils/Utils.h"

namespace Gadget{
	struct DX12_CommandFrame{
		explicit DX12_CommandFrame();
		DISABLE_COPY_AND_MOVE(DX12_CommandFrame)

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
		uint64_t fenceValue;

		[[nodiscard]] ErrorCode Wait(HANDLE fenceEvent_, ID3D12_Fence* fence_) const;
		void Release();
	};

	class DX12_Command{
	public:
		explicit DX12_Command(ID3D12_Device* const device_, D3D12_COMMAND_LIST_TYPE type_);
		~DX12_Command();
		DISABLE_COPY_AND_MOVE(DX12_Command); //Disable copy and move since this deals with raw DirectX pointers and objects

		[[nodiscard]] ErrorCode BeginFrame();
		[[nodiscard]] ErrorCode EndFrame(DX12_RenderSurface* renderSurface_);
		[[nodiscard]] ErrorCode Flush();

		ID3D12CommandQueue* const CommandQueue() const{ return cmdQueue.Get(); }
		ID3D12_GraphicsCommandList* const CommandList() const{ return cmdList.Get(); }
		constexpr uint32_t CurrentFrameIndex() const{ return frameIndex; }

	private:
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;
		Microsoft::WRL::ComPtr<ID3D12_GraphicsCommandList> cmdList;
		Microsoft::WRL::ComPtr<ID3D12_Fence> fence;
		uint64_t fenceValue; //A 32-bit uint would almost certainly be good enough, but 2 extra bytes gives us peace of mind
		HANDLE fenceEvent;
		DX12_CommandFrame cmdFrames[DX12::FrameBufferCount];
		uint32_t frameIndex;

		void Release();
	};
}

#endif //!GADGET_DX12_COMMAND_H
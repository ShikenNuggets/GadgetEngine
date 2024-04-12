#ifndef GADGET_DX12_UPLOAD_HANDLER_H
#define GADGET_DX12_UPLOAD_HANDLER_H

#include <wrl/client.h>

#include "GadgetEnums.h"
#include "Graphics/DX12/DX12_Defines.h"
#include "Utils/Utils.h"

namespace Gadget{
	struct DX12_UploadFrame{
		explicit DX12_UploadFrame();
		DISABLE_COPY_AND_MOVE(DX12_UploadFrame)

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
		Microsoft::WRL::ComPtr<ID3D12_GraphicsCommandList> cmdList;
		Microsoft::WRL::ComPtr<ID3D12_Resource> uploadBuffer;
		void* cpuAddress;
		uint64_t fenceValue;

		[[nodiscard]] ErrorCode WaitAndReset(HANDLE fenceEvent_, ID3D12_Fence* fence_);
		void Release();

		bool IsReady() const{ return uploadBuffer == nullptr; }
	};

	class DX12_UploadHandler{
	public:
		DX12_UploadHandler(ID3D12_Device* device_);
		DISABLE_COPY_AND_MOVE(DX12_UploadHandler);
		~DX12_UploadHandler();

		static constexpr uint32_t MaxUploads = 4;

	private:
		DX12_UploadFrame uploadFrames[MaxUploads];
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;
		Microsoft::WRL::ComPtr<ID3D12_Fence> fence;
		uint64_t fenceValue; //A 32-bit uint would almost certainly be good enough, but 2 extra bytes gives us peace of mind
		HANDLE fenceEvent; //TODO - This fence stuff sure looks familiar. Maybe we can make a reusable class for this?
	};
}

#endif //!GADGET_DX12_UPLOAD_HANDLER_H
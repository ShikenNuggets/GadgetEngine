#ifndef GADGET_DX12_UPLOAD_CONTEXT_H
#define GADGET_DX12_UPLOAD_CONTEXT_H

#include <cstdint>

#include "Graphics/DX12/DX12_Defines.h"
#include "Utils/Utils.h"

namespace Gadget{
	class DX12_UploadContext{
	public:
		DX12_UploadContext(uint32_t alignedSize_);
		DISABLE_COPY_AND_MOVE(DX12_UploadContext);
		~DX12_UploadContext();

		void EndUpload();

		ID3D12_GraphicsCommandList* const CommandList() const{ return cmdList; }
		ID3D12Resource* const UploadBuffer() const{ return uploadBuffer; }
		void* const CPUAddress() const{ return cpuAddress; }
		uint32_t FrameIndex() const{ return frameIndex; }

	private:
		ID3D12_GraphicsCommandList* cmdList;
		ID3D12Resource* uploadBuffer;
		void* cpuAddress;
		uint32_t frameIndex;
	};
}

#endif //!GADGET_DX12_UPLOAD_CONTEXT_H
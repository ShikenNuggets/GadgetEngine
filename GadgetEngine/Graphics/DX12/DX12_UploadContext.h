#ifndef GADGET_DX12_UPLOAD_CONTEXT_H
#define GADGET_DX12_UPLOAD_CONTEXT_H

#include <cstdint>

#include "Graphics/DX12/DX12_Defines.h"
#include "Utils/Utils.h"

namespace Gadget{
	class DX12_UploadContext{
	public:
		DX12_UploadContext(ID3D12_Device* device_, uint32_t alignedSize_);
		DISABLE_COPY_AND_MOVE(DX12_UploadContext);
		~DX12_UploadContext();

		void EndUpload();

		ID3D12_GraphicsCommandList* const CommandList() const;
		ID3D12Resource* const UploadBuffer() const;
		void* const CPUAddress() const;

	private:
		struct DX12_UploadFrame* uploadFrame;
	};
}

#endif //!GADGET_DX12_UPLOAD_CONTEXT_H
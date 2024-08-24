#ifndef GADGET_DX12_DESCRIPTOR_HEAP
#define GADGET_DX12_DESCRIPTOR_HEAP

#include <cstdint>
#include <memory>
#include <mutex>

#include <d3d12.h>

#include "GadgetEnums.h"
#include "Graphics/DX12/DX12_Defines.h"
#include "Utils/Utils.h"

namespace Gadget{
	struct DX12_DescriptorHandle{
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle{};
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

		constexpr bool IsValid() const{ return cpuHandle.ptr != NULL; }
		constexpr bool IsShaderVisible() const{ return gpuHandle.ptr != NULL; }
		constexpr uint32_t GetIndex() const{ return index; }

		void SetIndex(uint32_t index_){ index = index_; }

	private:
		uint32_t index = std::numeric_limits<uint32_t>::max(); //TODO - Check how much memory we could save by calculating this on the fly

#ifdef GADGET_DEBUG
		friend class DX12_DescriptorHeap;
		DX12_DescriptorHeap* container = nullptr;
#endif //GADGET_DEBUG
	};

	class DX12_DescriptorHeap{
	public:
		explicit DX12_DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_);
		virtual ~DX12_DescriptorHeap();
		DISABLE_COPY_AND_MOVE(DX12_DescriptorHeap)

		ErrorCode Initialize(ID3D12_Device* const device_, uint32_t capacity_, bool isShaderVisible_);
		void ProcessDeferredFree(uint32_t frameIndex_);
		void Release();

		[[nodiscard]] DX12_DescriptorHandle Allocate();
		void Free(DX12_DescriptorHandle& handle_);

		constexpr D3D12_DESCRIPTOR_HEAP_TYPE Type() const{ return type; }
		constexpr D3D12_CPU_DESCRIPTOR_HANDLE CPUStart() const{ return cpuStart; }
		constexpr D3D12_GPU_DESCRIPTOR_HANDLE GPUStart() const{ return gpuStart; }
		constexpr ID3D12DescriptorHeap* Heap() const{ return heap; }
		constexpr uint32_t Capacity() const{ return capacity; }
		constexpr uint32_t Size() const{ return size; }
		constexpr uint32_t DescriptorSize() const{ return descriptorSize; }
		constexpr bool IsShaderVisible() const{ return gpuStart.ptr != 0; }

	protected:
		ID3D12DescriptorHeap* heap; //NOT a ComPtr since the release needs to be deferred

	private:
		const D3D12_DESCRIPTOR_HEAP_TYPE type;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuStart;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuStart;
		std::unique_ptr<uint32_t[]> freeHandles;
		std::vector<uint32_t> deferredFreeIndices[3]; //TODO - Remove this magic number. Referencing DX12::FrameBufferCount in this header would be a circular dependency
		std::mutex mutex;
		uint32_t capacity;
		uint32_t size;
		uint32_t descriptorSize;
	};
}

#endif //!GADGET_DX12_DESCRIPTOR_HEAP
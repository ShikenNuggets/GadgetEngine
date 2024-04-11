#include "DX12_DescriptorHeap.h"

#include "Graphics/DX12/DX12.h"

using namespace Gadget;

DX12_DescriptorHeap::DX12_DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_) : type(type_), heap(nullptr), cpuStart(), gpuStart(), freeHandles(), deferredFreeIndices(), capacity(0), size(0), descriptorSize(0){}

ErrorCode DX12_DescriptorHeap::Initialize(ID3D12_Device* const device_, uint32_t capacity_, bool isShaderVisible_){
	std::lock_guard lock{ mutex };

	GADGET_BASIC_ASSERT(device_ != nullptr);
	GADGET_BASIC_ASSERT(capacity_ > 0);
	GADGET_BASIC_ASSERT(capacity_ < D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2);
	GADGET_BASIC_ASSERT(!(type == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER && capacity_ > D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE));

	if(device_ == nullptr || capacity_ == 0){
		Debug::Log(SID("RENDER"), "Tried to initialize DX12_DescriptorHeap with invalid arguments", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::Invalid_Args;
	}

	if(type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV || type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV){
		isShaderVisible_ = false;
	}

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags = isShaderVisible_ ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NumDescriptors = capacity_;
	desc.Type = type;
	desc.NodeMask = 0;

	HRESULT result = device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if(FAILED(result) || heap == nullptr){
		Debug::Log(SID("RENDER"), "An error occured while creating the descriptor heap!", Debug::Error, __FILE__, __LINE__);
		Release();
		return ErrorCode::D3D12_Error;
	}

	freeHandles = std::move(std::make_unique<uint32_t[]>(capacity_));
	capacity = capacity_;
	size = 0;

	for(uint32_t i = 0; i < capacity; i++){
		freeHandles[i] = i;
	}

	for(uint32_t i = 0; i < DX12::FrameBufferCount; i++){
		GADGET_BASIC_ASSERT(deferredFreeIndices[i].empty());

		if(!deferredFreeIndices[i].empty()){
			Debug::Log(SID("RENDER"), "Memory leak detected in DX12_DescriptorHeap::Initialize", Debug::Warning, __FILE__, __LINE__);
			deferredFreeIndices[i].clear(); //Clearing this is not good, but memory leak is better than garbage data
		}
	}

	descriptorSize = device_->GetDescriptorHandleIncrementSize(type);
	cpuStart = heap->GetCPUDescriptorHandleForHeapStart();
	if(isShaderVisible_){
		gpuStart = heap->GetGPUDescriptorHandleForHeapStart();
	}

	return ErrorCode::OK;
}

void DX12_DescriptorHeap::ProcessDeferredFree(uint32_t frameIndex_){
	std::lock_guard lock{ mutex };
	GADGET_BASIC_ASSERT(frameIndex_ < DX12::FrameBufferCount);

	std::vector<uint32_t>& indices{ deferredFreeIndices[frameIndex_] };
	for(const auto i : indices){
		size--;
		freeHandles[size] = i;
	}
	indices.clear();
}

void DX12_DescriptorHeap::Release(){
	if(heap != nullptr){
		DX12::GetInstance().DeferredRelease(heap);
		heap = nullptr;
	}
}

DX12_DescriptorHandle DX12_DescriptorHeap::Allocate(){
	std::lock_guard lock{ mutex };

	GADGET_BASIC_ASSERT(heap != nullptr);
	GADGET_BASIC_ASSERT(size < capacity);

	const uint32_t index = freeHandles[size];
	const uint32_t offset = index * descriptorSize;
	size++;

	DX12_DescriptorHandle handle;
	handle.cpuHandle.ptr = cpuStart.ptr + offset;

	if(IsShaderVisible()){
		handle.gpuHandle.ptr = gpuStart.ptr + offset;
	}

	handle.SetIndex(index);
#ifdef GADGET_DEBUG
	handle.container = this;
#endif //GADGET_DEBUG

	return handle;
}

void DX12_DescriptorHeap::Free(DX12_DescriptorHandle& handle_){
	if(!handle_.IsValid()){
		return;
	}

	std::lock_guard lock{ mutex };

	GADGET_BASIC_ASSERT(heap != nullptr);
	GADGET_BASIC_ASSERT(size > 0);
	GADGET_BASIC_ASSERT(handle_.cpuHandle.ptr >= cpuStart.ptr);
	GADGET_BASIC_ASSERT((handle_.cpuHandle.ptr - cpuStart.ptr) % descriptorSize == 0);

	const uint32_t index = static_cast<uint32_t>((handle_.cpuHandle.ptr - cpuStart.ptr)) / descriptorSize;

	GADGET_BASIC_ASSERT(handle_.GetIndex() < capacity);
	GADGET_BASIC_ASSERT(handle_.GetIndex() == index);
#ifdef GADGET_DEBUG
	GADGET_BASIC_ASSERT(handle_.container == this);
#endif //GADGET_DEBUG

	const uint32_t frameIndex = 0;
	deferredFreeIndices[frameIndex].push_back(index);
}
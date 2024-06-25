#include "DX12_UploadContext.h"

#include "Debug.h"
#include "Graphics/DX12/DX12_Helpers.h"
#include "Graphics/DX12/DX12_UploadHandler.h"

using namespace Gadget;

DX12_UploadContext::DX12_UploadContext(ID3D12_Device* device_, uint64_t alignedSize_) : uploadFrame(nullptr){
	DX12_UploadHandler& uploadHandler = DX12_UploadHandler::GetInstance();
	uploadFrame = uploadHandler.ReserveAvailableUploadFrame();

	uploadFrame->uploadBuffer = DX12_Helpers::CreateBuffer(device_, nullptr, alignedSize_, true);
	uploadFrame->uploadBuffer->SetName((L"Upload Buffer - Size: " + std::to_wstring(alignedSize_)).c_str());

	const D3D12_RANGE range{};
	uploadFrame->uploadBuffer->Map(0, &range, reinterpret_cast<void**>(&uploadFrame->cpuAddress));
	GADGET_BASIC_ASSERT(uploadFrame->cpuAddress != nullptr);

	GADGET_BASIC_ASSERT(uploadFrame->cmdList != nullptr);
	GADGET_BASIC_ASSERT(uploadFrame->uploadBuffer != nullptr);
	GADGET_BASIC_ASSERT(uploadFrame->cpuAddress != nullptr);

	uploadFrame->cmdAllocator->Reset();
	uploadFrame->cmdList->Reset(uploadFrame->cmdAllocator.Get(), nullptr);
}

DX12_UploadContext::~DX12_UploadContext(){}

void DX12_UploadContext::EndUpload(){
	DX12_UploadHandler& uploadHandler = DX12_UploadHandler::GetInstance();
	auto err = uploadHandler.EndFrameUpload(uploadFrame);
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "An error occured while uploading the frame!", err, __FILE__, __LINE__);
	}

	uploadFrame = nullptr;
}

ID3D12_GraphicsCommandList* DX12_UploadContext::CommandList() const{
	GADGET_BASIC_ASSERT(uploadFrame != nullptr);
	if(uploadFrame == nullptr){
		return nullptr;
	}

	return uploadFrame->cmdList.Get();
}

ID3D12Resource* DX12_UploadContext::UploadBuffer() const{
	GADGET_BASIC_ASSERT(uploadFrame != nullptr);
	if(uploadFrame == nullptr){
		return nullptr;
	}

	return uploadFrame->uploadBuffer.Get();
}

void* DX12_UploadContext::CPUAddress() const{
	GADGET_BASIC_ASSERT(uploadFrame != nullptr);
	if(uploadFrame == nullptr){
		return nullptr;
	}

	return uploadFrame->cpuAddress;
}
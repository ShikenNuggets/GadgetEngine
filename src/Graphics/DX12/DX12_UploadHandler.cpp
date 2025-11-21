#include "DX12_UploadHandler.h"

#include "Debug.h"
#include "Concurrency/SpinLock.h"

using namespace Gadget;

//----------------------------------------------------------------------------------------------------//
//---------- DX12_UploadFrame ------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

DX12_UploadFrame::DX12_UploadFrame() : cmdAllocator(nullptr), cmdList(nullptr), uploadBuffer(nullptr), cpuAddress(nullptr), fenceValue(0){}

[[nodiscard]] ErrorCode DX12_UploadFrame::WaitAndReset(HANDLE fenceEvent_, ID3D12_Fence* fence_){
	GADGET_BASIC_ASSERT(fence_ != nullptr);
	GADGET_BASIC_ASSERT(fenceEvent_ != nullptr);

	if(fence_->GetCompletedValue() < fenceValue){
		fence_->SetEventOnCompletion(fenceValue, fenceEvent_);
		WaitForSingleObject(fenceEvent_, INFINITE); //TODO - Infinite may not be wise here
	}

	uploadBuffer.Reset();
	cpuAddress = nullptr;

	return ErrorCode::OK;
}

void DX12_UploadFrame::Release(){
	cmdList.Reset();
	cmdAllocator.Reset();
}

//----------------------------------------------------------------------------------------------------//
//---------- DX12_UploadHandler ----------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

std::unique_ptr<DX12_UploadHandler> DX12_UploadHandler::instance = nullptr;

DX12_UploadHandler::DX12_UploadHandler(ID3D12_Device* device_) : uploadFrames(), cmdQueue(nullptr), fence(nullptr), fenceValue(0), fenceEvent(nullptr), frameMutex(), queueMutex(){
	GADGET_BASIC_ASSERT(device_ != nullptr);

	HRESULT hr = S_OK;
	for(uint32_t i = 0; i < MaxUploads; i++){
		DX12_UploadFrame& frame = uploadFrames[i];
		hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS(frame.cmdAllocator.ReleaseAndGetAddressOf()));
		if(FAILED(hr) || frame.cmdAllocator == nullptr){
			Debug::ThrowFatalError(SID("RENDER"), "Could not create command allocator for upload frame!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
		}

		hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COPY, frame.cmdAllocator.Get(), nullptr, IID_PPV_ARGS(frame.cmdList.ReleaseAndGetAddressOf()));
		if(FAILED(hr) || frame.cmdList == nullptr){
			Debug::ThrowFatalError(SID("RENDER"), "Could not create command allocator for upload frame!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
		}

		frame.cmdList->Close();
		frame.cmdAllocator->SetName((L"Upload Command Allocator " + std::to_wstring(i)).c_str());
		frame.cmdList->SetName((L"Upload Command List " + std::to_wstring(i)).c_str());
	}

	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type = D3D12_COMMAND_LIST_TYPE_COPY;

	hr = device_->CreateCommandQueue(&desc, IID_PPV_ARGS(cmdQueue.ReleaseAndGetAddressOf()));
	if(FAILED(hr) || cmdQueue == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create upload command queue!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
	cmdQueue->SetName(L"Upload Command Queue");

	hr = device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));
	if(FAILED(hr) || fence == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create upload fence!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
	fence->SetName(L"Upload Fence");

	fenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
	GADGET_BASIC_ASSERT(fenceEvent != nullptr);
}

DX12_UploadHandler::~DX12_UploadHandler(){
	if(fenceEvent != nullptr){
		CloseHandle(fenceEvent);
		fenceEvent = nullptr;
	}

	//Not strictly necessary but good form to put these all here anyway
	for(uint32_t i = 0; i < MaxUploads; i++){
		uploadFrames[i].Release();
	}

	fenceValue = 0;
	fence.Reset();
	cmdQueue.Reset();
}

DX12_UploadHandler& DX12_UploadHandler::GetInstance(ID3D12_Device* device_){
	GADGET_BASIC_ASSERT(instance != nullptr || device_ != nullptr);
	if(instance == nullptr){
		instance = std::make_unique<DX12_UploadHandler>(device_);
	}

	GADGET_BASIC_ASSERT(instance != nullptr);
	return *instance;
}

void DX12_UploadHandler::DeleteInstance(){
	instance.reset();
}

RaceConditionDetector gSpinLock; //TODO - This is not ideal

DX12_UploadFrame* DX12_UploadHandler::ReserveAvailableUploadFrame(){
	std::lock_guard lock{ frameMutex };

	for(uint32_t i = 0; i < MaxUploads; i++){
		if(uploadFrames[i].IsReady()){
			return &uploadFrames[i];
		}
	}

	int index = 0;
	while(!uploadFrames[index].IsReady()){
		index = (index + 1) % MaxUploads;
		BEGIN_RACE_CONDITION_DETECTION(gSpinLock);
	}

	END_RACE_CONDITION_DETECTION(gSpinLock);

	GADGET_BASIC_ASSERT(index < MaxUploads);

	uploadFrames[index].uploadBuffer = (ID3D12_Resource*)1; //TODO - This feels REALLY GROSS
	return &uploadFrames[index];
}

ErrorCode DX12_UploadHandler::EndFrameUpload(DX12_UploadFrame* frame_){
	GADGET_BASIC_ASSERT(frame_ != nullptr);
	if(frame_ == nullptr){
		return ErrorCode::Invalid_Args;
	}

	frame_->cmdList->Close();

	std::lock_guard lock{ queueMutex };
	ID3D12CommandList* const cmdLists[]{ frame_->cmdList.Get() };
	cmdQueue->ExecuteCommandLists((uint32_t)std::size(cmdLists), cmdLists);

	fenceValue++;
	cmdQueue->Signal(fence.Get(), fenceValue);

	return frame_->WaitAndReset(fenceEvent, fence.Get());
}
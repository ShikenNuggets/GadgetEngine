#include "DX12_Command.h"

using namespace Gadget;

//----------------------------------------------------------------------------------------------------//
//---------- DX12_CommandFrame -----------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

DX12_CommandFrame::DX12_CommandFrame() : cmdAllocator(nullptr), fenceValue(0){}

void DX12_CommandFrame::Wait(HANDLE fenceEvent_, ID3D12Fence1* fence_) const{
	GADGET_BASIC_ASSERT(fence_ != nullptr);
	GADGET_BASIC_ASSERT(fenceEvent_ != nullptr);

	if(fence_->GetCompletedValue() < fenceValue){
		HRESULT result = fence_->SetEventOnCompletion(fenceValue, fenceEvent_);
		if(FAILED(result)){
			Debug::ThrowFatalError(SID("RENDER"), "ID3D12Fence1::SetEventOnCompletion failed!", __FILE__, __LINE__);
		}
		WaitForSingleObject(fenceEvent_, INFINITE); //TODO - Not sure if infinite is wise here
	}
}

void DX12_CommandFrame::Release(){
	if(cmdAllocator != nullptr){
		cmdAllocator->Release();
		cmdAllocator = nullptr;
	}
	fenceValue = 0;
}

//----------------------------------------------------------------------------------------------------//
//---------- DX12_Command ----------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

DX12_Command::DX12_Command(ID3D12Device8* const device_, D3D12_COMMAND_LIST_TYPE type_) : cmdQueue(nullptr), cmdList(nullptr), fence(nullptr), fenceValue(0), fenceEvent(nullptr), cmdFrames(), frameIndex(0){
	std::wstring typeNamePrefix;
	if(type_ == D3D12_COMMAND_LIST_TYPE_DIRECT){
		typeNamePrefix += L"GFX Command ";
	}else if(type_ == D3D12_COMMAND_LIST_TYPE_COMPUTE){
		typeNamePrefix += L"Compute Command ";
	}else{
		typeNamePrefix += L"Command ";
	}

	GADGET_BASIC_ASSERT(device_ != nullptr);

	//Create Command Queue
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type = type_;

	HRESULT result = device_->CreateCommandQueue(&desc, IID_PPV_ARGS(&cmdQueue));
	if(FAILED(result) || cmdQueue == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "", __FILE__, __LINE__);
	}
	cmdQueue->SetName((typeNamePrefix + L"Queue").c_str());

	//Create Command Frames
	for(int i = 0; i < DX12::FrameBufferCount; i++){
		DX12_CommandFrame& frame = cmdFrames[i];
		result = device_->CreateCommandAllocator(type_, IID_PPV_ARGS(&frame.cmdAllocator));
		if(FAILED(result) || frame.cmdAllocator == nullptr){
			Debug::ThrowFatalError(SID("RENDER"), "ID3D12Device8::CreateCommandAllocator failed!", __FILE__, __LINE__);
		}
		frame.cmdAllocator->SetName((typeNamePrefix + L"Allocator " + std::to_wstring(i)).c_str());
	}

	GADGET_BASIC_ASSERT(cmdFrames[0].cmdAllocator != nullptr);

	result = device_->CreateCommandList(0, type_, cmdFrames[0].cmdAllocator, nullptr, IID_PPV_ARGS(&cmdList));
	if(FAILED(result) || cmdList == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "ID3D12Device8::CreateCommandList failed!", __FILE__, __LINE__);
	}
	cmdList->SetName((typeNamePrefix + L"List").c_str());
	cmdList->Close();

	//Create Fence
	result = device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if(FAILED(result) || fence == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "ID3D12Device8::CreateFence failed!", __FILE__, __LINE__);
	}
	fence->SetName(L"D3D12 Fence");

	//Create Fence Event
	fenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
	if(fenceEvent == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "CreateEventEx failed!", __FILE__, __LINE__);
	}
}

DX12_Command::~DX12_Command(){
	Release();
}

void DX12_Command::BeginFrame(){
	GADGET_BASIC_ASSERT(frameIndex < DX12::FrameBufferCount);
	GADGET_BASIC_ASSERT(cmdFrames[frameIndex].cmdAllocator != nullptr);

	cmdFrames[frameIndex].Wait(fenceEvent, fence);

	HRESULT result = cmdFrames[frameIndex].cmdAllocator->Reset();
	if(FAILED(result)){
		Debug::ThrowFatalError(SID("RENDER"), "ID3D12CommandAllocator::Reset failed!", __FILE__, __LINE__);
	}

	result = cmdList->Reset(cmdFrames[frameIndex].cmdAllocator, nullptr);
	if(FAILED(result)){
		Debug::ThrowFatalError(SID("RENDER"), "ID3D12GraphicsCommandList6::Reset failed!", __FILE__, __LINE__);
	}
}

void DX12_Command::EndFrame(){
	cmdList->Close();
	ID3D12CommandList* const cmdLists[]{ cmdList };
	cmdQueue->ExecuteCommandLists(_countof(cmdLists), &cmdLists[0]);

	uint64_t& fv = fenceValue;
	++fv;
	cmdFrames[frameIndex].fenceValue = fv;
	cmdQueue->Signal(fence, fv);

	frameIndex = (frameIndex + 1) % DX12::FrameBufferCount;
}

void DX12_Command::Flush(){
	for(int i = 0; i < DX12::FrameBufferCount; i++){
		cmdFrames[i].Wait(fenceEvent, fence);
	}
	frameIndex = 0;
}

void DX12_Command::Release(){
	Flush();

	if(fence != nullptr){
		fence->Release();
		fence = nullptr;
		fenceValue = 0;
	}

	if(fenceEvent != nullptr){
		CloseHandle(fenceEvent);
		fenceEvent = nullptr;
	}

	if(cmdQueue != nullptr){
		cmdQueue->Release();
		cmdQueue = nullptr;
	}

	if(cmdList != nullptr){
		cmdList->Release();
		cmdList = nullptr;
	}

	for(int i = 0; i < DX12::FrameBufferCount; i++){
		cmdFrames[i].Release();
	}
}
#include "DX12_Command.h"

using namespace Gadget;

//----------------------------------------------------------------------------------------------------//
//---------- DX12_CommandFrame -----------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

DX12_CommandFrame::DX12_CommandFrame() : cmdAllocator(nullptr), fenceValue(0){}

ErrorCode DX12_CommandFrame::Wait(HANDLE fenceEvent_, ID3D12Fence1* fence_) const{
	GADGET_BASIC_ASSERT(fence_ != nullptr);
	GADGET_BASIC_ASSERT(fenceEvent_ != nullptr);

	if(fence_->GetCompletedValue() < fenceValue){
		HRESULT result = fence_->SetEventOnCompletion(fenceValue, fenceEvent_);
		if(FAILED(result)){
			Debug::Log(SID("RENDER"), "ID3D12Fence1::SetEventOnCompletion failed!", Debug::Error, __FILE__, __LINE__);
			return ErrorCode::D3D12_Error;
		}

		DWORD err = WaitForSingleObject(fenceEvent_, INFINITE); //TODO - Infinite may not be wise here
		if(err != 0){
			Debug::Log(SID("RENDER"), "An error occurred while waiting for the fence event!", Debug::Error, __FILE__, __LINE__);
			return ErrorCode::D3D12_Error;
		}
	}

	return ErrorCode::OK;
}

void DX12_CommandFrame::Release(){
	cmdAllocator.Reset();
	fenceValue = 0;
}

//----------------------------------------------------------------------------------------------------//
//---------- DX12_Command ----------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

DX12_Command::DX12_Command(ID3D12_Device* const device_, D3D12_COMMAND_LIST_TYPE type_) : cmdQueue(nullptr), cmdList(nullptr), fence(nullptr), fenceValue(0), fenceEvent(nullptr), cmdFrames(), frameIndex(0){
	std::wstring typeNamePrefix;
	if(type_ == D3D12_COMMAND_LIST_TYPE_DIRECT){
		typeNamePrefix += L"GFX Command ";
	}else if(type_ == D3D12_COMMAND_LIST_TYPE_COMPUTE){
		typeNamePrefix += L"Compute Command ";
	}else{
		typeNamePrefix += L"Command ";
	}

	GADGET_BASIC_ASSERT(device_ != nullptr);
	if(device_ == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Tried creating command list with null device!", ErrorCode::Invalid_Args, __FILE__, __LINE__);
	}

	//Create Command Queue
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type = type_;

	HRESULT result = device_->CreateCommandQueue(&desc, IID_PPV_ARGS(cmdQueue.ReleaseAndGetAddressOf()));
	if(FAILED(result) || cmdQueue == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create command queue!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
	cmdQueue->SetName((typeNamePrefix + L"Queue").c_str());

	//Create Command Frames
	for(int i = 0; i < DX12::FrameBufferCount; i++){
		DX12_CommandFrame& frame = cmdFrames[i];
		result = device_->CreateCommandAllocator(type_, IID_PPV_ARGS(frame.cmdAllocator.ReleaseAndGetAddressOf()));
		if(FAILED(result) || frame.cmdAllocator == nullptr){
			Debug::ThrowFatalError(SID("RENDER"), "ID3D12Device8::CreateCommandAllocator failed!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
		}

		frame.cmdAllocator->SetName((typeNamePrefix + L"Allocator " + std::to_wstring(i)).c_str());
	}

	GADGET_BASIC_ASSERT(cmdFrames[0].cmdAllocator != nullptr);

	result = device_->CreateCommandList(0, type_, cmdFrames[0].cmdAllocator.Get(), nullptr, IID_PPV_ARGS(cmdList.ReleaseAndGetAddressOf()));
	if(FAILED(result) || cmdList == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "ID3D12Device8::CreateCommandList failed!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
	cmdList->SetName((typeNamePrefix + L"List").c_str());

	//Create Fence
	result = device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));
	if(FAILED(result) || fence == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "ID3D12Device8::CreateFence failed!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
	fence->SetName(L"D3D12 Fence");

	//Create Fence Event
	fenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
	if(fenceEvent == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "CreateEventEx failed!", ErrorCode::Win32_Error, __FILE__, __LINE__);
	}
}

DX12_Command::~DX12_Command(){
	Release();
}

ErrorCode DX12_Command::BeginFrame(){
	GADGET_BASIC_ASSERT(cmdList != nullptr);
	GADGET_BASIC_ASSERT(frameIndex < DX12::FrameBufferCount);
	GADGET_BASIC_ASSERT(cmdFrames[frameIndex].cmdAllocator != nullptr);

	if(frameIndex >= DX12::FrameBufferCount){
		Debug::Log(SID("RENDER"), "Frame index was invalid", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::Invalid_State;
	}

	if(cmdList == nullptr || cmdFrames[frameIndex].cmdAllocator == nullptr){
		Debug::Log(SID("RENDER"), "Command list/allocators were not initialized correctly", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::Invalid_State;
	}

	auto err = cmdFrames[frameIndex].Wait(fenceEvent, fence.Get());
	if(err != ErrorCode::OK){
		Debug::Log(SID("RENDER"), "Command Frame Wait failed", Debug::Error, __FILE__, __LINE__);
		return err;
	}

	HRESULT result = cmdFrames[frameIndex].cmdAllocator->Reset();
	if(FAILED(result)){
		Debug::Log(SID("RENDER"), "ID3D12CommandAllocator Reset failed!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	result = cmdList->Reset(cmdFrames[frameIndex].cmdAllocator.Get(), nullptr);
	if(FAILED(result)){
		Debug::Log(SID("RENDER"), "ID3D12GraphicsCommandList Reset failed!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	return ErrorCode::OK;
}

ErrorCode DX12_Command::EndFrame(DX12_RenderSurface* renderSurface_){
	GADGET_ASSERT(renderSurface_ != nullptr, "Do not call DX12_Command::EndFrame without a render surface! Call ExecuteCommandsImmediate instead!");
	GADGET_BASIC_ASSERT(cmdList != nullptr);
	GADGET_BASIC_ASSERT(cmdQueue != nullptr);

	if(cmdList == nullptr || cmdQueue == nullptr){
		Debug::Log(SID("RENDER"), "Command list/queue were not initialized correctly");
		return ErrorCode::Invalid_State;
	}

	if(FAILED(cmdList->Close())){
		Debug::Log("Could not close the command list before ending the frame", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	ID3D12CommandList* const cmdLists[]{ cmdList.Get() };
	cmdQueue->ExecuteCommandLists(static_cast<uint32_t>(std::size(cmdLists)), &cmdLists[0]);

	if(renderSurface_ != nullptr){
		auto err = renderSurface_->Present();
		if(err != ErrorCode::OK){
			Debug::Log("Could not present the render surface", Debug::Error, __FILE__, __LINE__);
			return err;
		}
	}

	const uint64_t fv = ++fenceValue;
	cmdFrames[frameIndex].fenceValue = fv;

	if(FAILED(cmdQueue->Signal(fence.Get(), fv))){
		Debug::Log("Could not signal the command queue", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	Debug::Log("Frame Index (Before): " + std::to_string(frameIndex));
	if(renderSurface_ != nullptr){
		frameIndex = renderSurface_->CurrentBackBufferIndex();
	}else{
		Debug::Log("Test");
		frameIndex = (frameIndex + 1) % DX12::FrameBufferCount;
	}
	Debug::Log("Frame Index (After): " + std::to_string(frameIndex));

	GADGET_BASIC_ASSERT(frameIndex < DX12::FrameBufferCount);

	return ErrorCode::OK;
}

ErrorCode DX12_Command::Flush(){
	for(int i = 0; i < DX12::FrameBufferCount; i++){
		auto err = cmdFrames[i].Wait(fenceEvent, fence.Get());
		if(err != ErrorCode::OK){
			Debug::Log("An error occurred while flushing command frames", Debug::Error, __FILE__, __LINE__);
			return err;
		}
	}

	frameIndex = 0;

	return ErrorCode::OK;
}

ErrorCode DX12_Command::CloseList(){
	GADGET_BASIC_ASSERT(cmdList != nullptr);
	HRESULT result = cmdList->Close();
	if(FAILED(result)){
		GADGET_LOG_ERROR(SID("RENDER"), "Could not close the command list!");
		return ErrorCode::D3D12_Error;
	}

	return ErrorCode::OK;
}

ErrorCode DX12_Command::ExecuteCommandsImmediate(){
	auto err = CloseList();
	if(err != ErrorCode::OK){
		return err;
	}

	ID3D12CommandList* ppCommandLists[] = { CommandList() };
	static_assert(std::size(ppCommandLists) > 0);
	cmdQueue->ExecuteCommandLists(std::size(ppCommandLists), &ppCommandLists[0]);

	const uint64_t fv = ++fenceValue;
	HRESULT hr = cmdQueue->Signal(fence.Get(), fv);
	if(FAILED(hr)){
		GADGET_LOG_ERROR(SID("RENDER"), "An error occurred on signalling the command queue fence!");
		return ErrorCode::D3D12_Error;
	}

	hr = fence->SetEventOnCompletion(fenceValue, fenceEvent);
	if(FAILED(hr)){
		GADGET_LOG_ERROR(SID("RENDER"), "Could not set fence event!");
		return ErrorCode::D3D12_Error;
	}

	WaitForSingleObject(fenceEvent, INFINITE);

	hr = cmdList->Reset(cmdFrames[frameIndex].cmdAllocator.Get(), nullptr);
	if(FAILED(hr)){
		return ErrorCode::D3D12_Error;
	}

	return ErrorCode::OK;
}

void DX12_Command::Release(){
	auto err = Flush();
	if(err != ErrorCode::OK){
		Debug::Log("Command frames could not be flushed before shutdown", Debug::Warning, __FILE__, __LINE__);
		//Not continuing from here would just mean memory leaks... might as well try to continue as normal
	}

	fence.Reset();

	if(fenceEvent != nullptr){
		CloseHandle(fenceEvent);
		fenceEvent = nullptr;
	}

	cmdQueue.Reset();
	cmdList.Reset();

	for(int i = 0; i < DX12::FrameBufferCount; i++){
		cmdFrames[i].Release();
	}
}
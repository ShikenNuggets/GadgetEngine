#ifndef GADGET_DX12_COMMAND_H
#define GADGET_DX12_COMMAND_H

#include <d3d12.h>

#include "Debug.h"

namespace Gadget{
	constexpr uint32_t DX12_FrameBufferCount = 3; //TODO - Find somewhere more appropriate to put this - Should this be configurable?

	struct DX12_CommandFrame{
		ID3D12CommandAllocator* cmdAllocator = nullptr;

		void Wait(){}

		void Release(){
			if(cmdAllocator != nullptr){
				cmdAllocator->Release();
				cmdAllocator = nullptr;
			}
		}
	};

	class DX12_Command{
		explicit DX12_Command(ID3D12Device8* const device_, D3D12_COMMAND_LIST_TYPE type_){
			std::wstring typeNamePrefix;
			if(type_ == D3D12_COMMAND_LIST_TYPE_DIRECT){
				typeNamePrefix += L"GFX Command ";
			}else if(type_ == D3D12_COMMAND_LIST_TYPE_COMPUTE){
				typeNamePrefix += L"Compute Command ";
			}else{
				typeNamePrefix += L"Command ";
			}

			GADGET_BASIC_ASSERT(device_ != nullptr);

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

			for(int i = 0; i < DX12_FrameBufferCount; i++){
				DX12_CommandFrame& frame = cmdFrames[i];
				result = device_->CreateCommandAllocator(type_, IID_PPV_ARGS(&frame.cmdAllocator));
				if(FAILED(result) || frame.cmdAllocator == nullptr){
					Debug::ThrowFatalError(SID("RENDER"), "", __FILE__, __LINE__);
				}
				frame.cmdAllocator->SetName((typeNamePrefix + L"Allocator " + std::to_wstring(i)).c_str());
			}
			
			result = device_->CreateCommandList(0, type_, cmdFrames[0].cmdAllocator, nullptr, IID_PPV_ARGS(&cmdList));
			if(FAILED(result) || cmdList == nullptr){
				Debug::ThrowFatalError(SID("RENDER"), "", __FILE__, __LINE__);
			}
			cmdList->SetName((typeNamePrefix + L"List").c_str());
		}

	private:
		void BeginFrame(){
			DX12_CommandFrame& frame{ cmdFrames[frameIndex] };
			frame.Wait();

			HRESULT result = frame.cmdAllocator->Reset();
			if(FAILED(result)){
				Debug::ThrowFatalError(SID("RENDER"), "", __FILE__, __LINE__);
			}

			result = cmdList->Reset(frame.cmdAllocator, nullptr);
			if(FAILED(result)){
				Debug::ThrowFatalError(SID("RENDER"), "", __FILE__, __LINE__);
			}
		}

		void EndFrame(){
			cmdList->Close();
			ID3D12CommandList* const cmdLists[]{ cmdList };
			cmdQueue->ExecuteCommandLists(_countof(cmdLists), &cmdLists[0]);
			frameIndex = (frameIndex + 1) % DX12_FrameBufferCount;
		}

		void Release(){}

		ID3D12CommandQueue* cmdQueue;
		ID3D12GraphicsCommandList6* cmdList;
		DX12_CommandFrame cmdFrames[DX12_FrameBufferCount]{};
		uint32_t frameIndex = 0;
	};
}

#endif //!GADGET_DX12_COMMAND_H
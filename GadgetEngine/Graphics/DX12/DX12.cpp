#include "DX12.h"

#include "DX12_Command.h"

using namespace Gadget;

ID3D12Device8* DX12::mainDevice = nullptr;
DX12_Command* DX12::gfxCommand = nullptr;
uint32_t DX12::deferredReleaseFlag[DX12::FrameBufferCount]{};

bool DX12::IsInitialized(){ return mainDevice != nullptr && gfxCommand != nullptr; }

ID3D12Device8* const DX12::MainDevice(){ return mainDevice; }

uint32_t DX12::CurrentFrameIndex(){
	if(!IsInitialized()){
		return 0;
	}

	return gfxCommand->CurrentFrameIndex();
}

void DX12::SetDeferredReleaseFlag(){
	deferredReleaseFlag[CurrentFrameIndex()] = 1;
}
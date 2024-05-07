#include "DXR_MaterialInfo.h"

#include "Graphics/DX12/DX12.h"

using namespace Gadget;

DXR_MaterialInfo::DXR_MaterialInfo(const std::vector<Color>& colors_) : MaterialInfo(), hitGroupIndex(0){
	GADGET_BASIC_ASSERT(colors_.size() > 0);
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());

	if(colors_.size() == 1){
		hitGroupIndex = 1;
	}

	constBuffer.Attach(DX12::GetInstance().CreateBuffer(colors_.data(), colors_.size() * sizeof(Color), true, D3D12_RESOURCE_STATE_GENERIC_READ));
	GADGET_BASIC_ASSERT(constBuffer != nullptr);
	constBuffer->SetName(L"Constant Buffer");
}
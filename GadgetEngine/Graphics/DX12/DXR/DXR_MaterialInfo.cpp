#include "DXR_MaterialInfo.h"

#include "Graphics/DX12/DX12.h"

using namespace Gadget;

DXR_MaterialInfo::DXR_MaterialInfo(const std::vector<Color>& colors_) : MaterialInfo(){
	GADGET_BASIC_ASSERT(colors_.size() > 0);
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());

	constBuffer.Attach(DX12::GetInstance().CreateBuffer(colors_.data(), colors_.size() * sizeof(Color), true, D3D12_RESOURCE_STATE_GENERIC_READ));
}
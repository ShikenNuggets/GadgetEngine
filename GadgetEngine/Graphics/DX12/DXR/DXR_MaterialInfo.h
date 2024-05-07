#ifndef GADGET_DXR_MATERIAL_INFO_H
#define GADGET_DXR_MATERIAL_INFO_H

#include <vector>

#include <d3d12.h>
#include <wrl/client.h>

#include "Graphics/Color.h"
#include "Graphics/DX12/DX12_Defines.h"
#include "Graphics/Materials/MaterialInfo.h"

namespace Gadget{
	class DXR_MaterialInfo : public MaterialInfo{
	public:
		DXR_MaterialInfo(const std::vector<Color>& colors_);
		virtual ~DXR_MaterialInfo() override = default;

		ID3D12_Resource* ConstBuffer(){ return constBuffer.Get(); }
		size_t HitGroupIndex(){ return hitGroupIndex; }

	protected:
		size_t hitGroupIndex;
		Microsoft::WRL::ComPtr<ID3D12_Resource> constBuffer;
	};
}

#endif //!GADGET_DXR_MATERIAL_INFO_H
#ifndef GADGET_DXR_OUTPUT_RESOURCE_H
#define GADGET_DXR_OUTPUT_RESOURCE_H

#include <d3d12.h>
#include <wrl/client.h>

#include "ScreenCoordinate.h"
#include "Graphics/DX12/DX12_Defines.h"

namespace Gadget{
	class DXR_OutputResource{
	public:
		DXR_OutputResource(const ScreenCoordinate& frameSize_);

		ID3D12_Resource* Resource(){ return outputResource.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D12_Resource> outputResource;
	};
}

#endif //!GADGET_DXR_OUTPUT_RESOURCE_H
#ifndef GADGET_DXR_SHADER_RESOURCE_HEAP_H
#define GADGET_DXR_SHADER_RESOURCE_HEAP_H

#include "Graphics/DX12/DX12_Defines.h"
#include "Graphics/DX12/DX12_DescriptorHeap.h"
#include "Graphics/DX12/DXR/DXR_OutputResource.h"
#include "Graphics/DX12/DXR/DXR_TopLevelAS.h"

namespace Gadget{
	class DXR_ShaderResourceHeap : public DX12_DescriptorHeap{
	public:
		DXR_ShaderResourceHeap(DXR_OutputResource* outputResource_, DXR_TopLevelAS* topLevelAS_);

		void CreateCBV(ID3D12_Resource* buffer_, size_t size_);

	private:
		DX12_DescriptorHandle outputResourceHandle;
		DX12_DescriptorHandle topLevelASHandle;
		std::vector<DX12_DescriptorHandle> constantBufferViews;
	};
}

#endif //!GADGET_DXR_SHADER_RESOURCE_HEAP_H
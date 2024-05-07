#ifndef GADGET_DXR_H
#define GADGET_DXR_H

#include <memory>
#include <string>

#include <d3d12.h>
#include <DirectXMath.h>
#include <dxcapi.h>
#include <wrl/client.h>

#include "GadgetEnums.h"
#include "ScreenCoordinate.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_DescriptorHeap.h"
#include "Graphics/DX12/DXR/DXR_MeshInfo.h"
#include "Graphics/DX12/DXR/DXR_OutputResource.h"
#include "Graphics/DX12/DXR/DXR_PipelineStateObject.h"
#include "Graphics/DX12/DXR/DXR_ShaderBindingTable.h"
#include "Graphics/DX12/DXR/DXR_ShaderResourceHeap.h"
#include "Graphics/DX12/DXR/DXR_TopLevelAS.h"
#include "Math/Matrix.h"
#include "Utils/Utils.h"

namespace Gadget{
	class DXR{
	public:
		DXR(ScreenCoordinate frameSize_, const std::vector<DXR_MeshInfo*>& meshInfo_);
		~DXR();
		DISABLE_COPY_AND_MOVE(DXR);

		static DXR& GetInstance();
		static DXR& GetInstance(ScreenCoordinate frameSize_, const std::vector<DXR_MeshInfo*>& meshInfo_);
		[[nodiscard]] static ErrorCode DeleteInstance();

		ID3D12StateObject* RTStateObject(){ return pso->StateObject(); }
		ID3D12Resource* OutputResource(){ return outputResource->Resource(); }
		ID3D12DescriptorHeap* Heap(){ return heap->Heap(); }
		nv_helpers_dx12::ShaderBindingTableGenerator& SBTHelper(){ return shaderBindingTable->SBTHelper(); }
		ID3D12Resource* SBTStorage() const{ return shaderBindingTable->Storage(); }

		void UpdateTopLevelAS();
		void CreateTopLevelAS(const std::vector<DXR_MeshInstance>& meshInstances_);

		void UpdateCameraBuffer(const Matrix4& view_, const Matrix4& perspective_);

	private:
		static std::unique_ptr<DXR> instance;

		void CreateShaderResourceHeap();
		void CreateShaderBindingTable();

		void CreateCameraBuffer();

		DX12& dx12;

		DXR_TopLevelAS* topLevelAS;
		std::vector<DXR_MeshInstance> instances;

		DXR_PipelineStateObject* pso;
		DXR_OutputResource* outputResource;

		DXR_ShaderResourceHeap* heap;
		DXR_ShaderBindingTable* shaderBindingTable;

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> colorConstBuffers;
		std::vector<DXR_MeshInfo*> meshInfos;
		Microsoft::WRL::ComPtr<ID3D12Resource> cameraBuffer;
		uint32_t cameraBufferSize;
	};
}

#endif //!GADGET_DXR_H
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
#include "Graphics/DX12/DXR/DXR_PipelineStateObject.h"
#include "Graphics/DX12/DXR/DXR_OutputResource.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/TopLevelASGenerator.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/ShaderBindingTableGenerator.h"
#include "Math/Matrix.h"
#include "Utils/Utils.h"

namespace Gadget{
	struct AccelerationStructureBuffers{
		Microsoft::WRL::ComPtr<ID3D12Resource> pScratch;
		Microsoft::WRL::ComPtr<ID3D12Resource> pResult;
		Microsoft::WRL::ComPtr<ID3D12Resource> pInstanceDesc;

		void SetName(const std::wstring& namePrefix_){
			if(pScratch != nullptr){
				pScratch->SetName((namePrefix_ + L"_Scratch").c_str());
			}

			if(pResult != nullptr){
				pResult->SetName((namePrefix_ + L"_Result").c_str());
			}

			if(pInstanceDesc != nullptr){
				pInstanceDesc->SetName((namePrefix_ + L"_InstanceDesc").c_str());
			}
		}
	};

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
		nv_helpers_dx12::ShaderBindingTableGenerator& SBTHelper(){ return sbtHelper; }
		ID3D12Resource* SBTStorage() const{ return sbtStorage.Get(); }

		void UpdateTopLevelAS();
		AccelerationStructureBuffers CreateBottomLevelAS(std::vector<std::pair<Microsoft::WRL::ComPtr<ID3D12Resource>, uint32_t>> vVertexBuffers_, std::vector<std::pair<Microsoft::WRL::ComPtr<ID3D12Resource>, uint32_t>> vIndexBuffers_);
		void CreateAccelerationStructures(const std::vector<Microsoft::WRL::ComPtr<ID3D12_Resource>>& resources_);

		void UpdateCameraBuffer(const Matrix4& view_, const Matrix4& perspective_);

	private:
		static std::unique_ptr<DXR> instance;

		void CreateTopLevelAS(const std::vector<std::pair<Microsoft::WRL::ComPtr<ID3D12Resource>, DirectX::XMMATRIX>>& instances_, bool updateOnly_ = false);

		void CreateShaderResourceHeap();
		void CreateShaderBindingTable();

		void CreateCameraBuffer();

		DX12& dx12;

		Microsoft::WRL::ComPtr<ID3D12Resource> bottomLevelAS;
		Microsoft::WRL::ComPtr<ID3D12Resource> bottomLevelAS2;
		nv_helpers_dx12::TopLevelASGenerator topLevelASGenerator;
		AccelerationStructureBuffers topLevelASBuffers;
		std::vector<std::pair<Microsoft::WRL::ComPtr<ID3D12Resource>, DirectX::XMMATRIX>> instances;

		DXR_PipelineStateObject* pso;
		DXR_OutputResource* outputResource;

		nv_helpers_dx12::ShaderBindingTableGenerator sbtHelper;
		Microsoft::WRL::ComPtr<ID3D12Resource> sbtStorage;

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> colorConstBuffers;
		std::vector<DXR_MeshInfo*> meshInfos;
		Microsoft::WRL::ComPtr<ID3D12Resource> cameraBuffer;
		uint32_t cameraBufferSize;
	};
}

#endif //!GADGET_DXR_H
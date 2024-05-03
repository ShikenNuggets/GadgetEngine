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
#include "Graphics/DX12/DXR/nv_helpers_dx12/TopLevelASGenerator.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/ShaderBindingTableGenerator.h"
#include "Graphics/DX12/DXR/DXR_MeshInfo.h"
#include "Math/Matrix.h"
#include "Utils/Utils.h"

namespace Gadget{
	struct AccelerationStructureBuffers{
		Microsoft::WRL::ComPtr<ID3D12Resource> pScratch;
		Microsoft::WRL::ComPtr<ID3D12Resource> pResult;
		Microsoft::WRL::ComPtr<ID3D12Resource> pInstanceDesc;
	};

	class DXR{
	public:
		DXR(ScreenCoordinate frameSize_, const std::vector<DXR_MeshInfo*>& meshInfo_);
		DISABLE_COPY_AND_MOVE(DXR);

		static DXR& GetInstance();
		static DXR& GetInstance(ScreenCoordinate frameSize_, const std::vector<DXR_MeshInfo*>& meshInfo_);
		[[nodiscard]] static ErrorCode DeleteInstance();

		ID3D12StateObject* RTStateObject(){ return rtStateObject.Get(); }
		ID3D12Resource* OutputResource(){ return outputResource.Get(); }
		nv_helpers_dx12::ShaderBindingTableGenerator& SBTHelper(){ return sbtHelper; }
		ID3D12Resource* SBTStorage() const{ return sbtStorage.Get(); }

		AccelerationStructureBuffers CreateBottomLevelAS(std::vector<std::pair<Microsoft::WRL::ComPtr<ID3D12Resource>, uint32_t>> vVertexBuffers_, std::vector<std::pair<Microsoft::WRL::ComPtr<ID3D12Resource>, uint32_t>> vIndexBuffers_);
		void CreateAccelerationStructures(const std::vector<Microsoft::WRL::ComPtr<ID3D12_Resource>>& resources_);

		void UpdateCameraBuffer(const Matrix4& view_, const Matrix4& perspective_);

	private:
		static std::unique_ptr<DXR> instance;

		void CreateTopLevelAS(const std::vector<std::pair<Microsoft::WRL::ComPtr<ID3D12Resource>, DirectX::XMMATRIX>>& instances_);

		Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRayGenSignature();
		Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateMissSignature();
		Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateHitSignature();

		void CreateRaytracingPipeline();
		void CreateRaytracingOutputBuffer();
		void CreateShaderResourceHeap();
		void CreateShaderBindingTable();

		void CreateCameraBuffer();

		DX12& dx12;
		ScreenCoordinate frameSize;

		Microsoft::WRL::ComPtr<ID3D12Resource> bottomLevelAS;
		Microsoft::WRL::ComPtr<ID3D12Resource> bottomLevelAS2;
		nv_helpers_dx12::TopLevelASGenerator topLevelASGenerator;
		AccelerationStructureBuffers topLevelASBuffers;
		std::vector<std::pair<Microsoft::WRL::ComPtr<ID3D12Resource>, DirectX::XMMATRIX>> instances;

		D3D12_SHADER_BYTECODE rayGenLibrary;
		D3D12_SHADER_BYTECODE hitLibrary;
		D3D12_SHADER_BYTECODE missLibrary;

		Microsoft::WRL::ComPtr<ID3D12RootSignature> rayGenSignature;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> hitSignature;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> missSignature;

		Microsoft::WRL::ComPtr<ID3D12StateObject> rtStateObject;
		Microsoft::WRL::ComPtr<ID3D12StateObjectProperties> rtStateObjectProperties;

		Microsoft::WRL::ComPtr<ID3D12Resource> outputResource;

		nv_helpers_dx12::ShaderBindingTableGenerator sbtHelper;
		Microsoft::WRL::ComPtr<ID3D12Resource> sbtStorage;

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> vertexBuffers;
		std::vector<DXR_MeshInfo*> meshInfos;
		Microsoft::WRL::ComPtr<ID3D12Resource> cameraBuffer;
		uint32_t cameraBufferSize;
	};
}

#endif //!GADGET_DXR_H
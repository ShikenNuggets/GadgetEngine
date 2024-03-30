#ifndef GADGET_DX12_HELPERS_H
#define GADGET_DX12_HELPERS_H

#include <cstdint>

#include <dxgi1_6.h>
#include <d3d12.h>

#include "Debug.h"

namespace Gadget{
	//Some of these are loosely based on Microsoft's d3dx12 helper library: https://github.com/microsoft/DirectX-Headers/blob/main/include/directx/d3dx12.h
	//TODO - I wanted to implement these myself while I'm learning how DX12 works, but we should eventually start using the "official" helpers once I'm more comfortable with it
	namespace DX12_Helpers{
		//----------------------------------------------------------------------------------------------------//
		//---------- Default Properties ----------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//

		constexpr D3D12_HEAP_PROPERTIES DefaultHeapProperties{
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			0,	//CreationNodeMask
			0	//VisibleNodeMask
		};

		//----------------------------------------------------------------------------------------------------//
		//---------- DX12_DescriptorRange --------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//

		struct DX12_DescriptorRange : public D3D12_DESCRIPTOR_RANGE1{
			constexpr explicit DX12_DescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE rangeType_,
				uint32_t descriptorCount_, uint32_t shaderRegister_, uint32_t space_ = 0,
				D3D12_DESCRIPTOR_RANGE_FLAGS flags_ = D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
				uint32_t offsetFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
			) : D3D12_DESCRIPTOR_RANGE1{ rangeType_, descriptorCount_, shaderRegister_, space_, flags_, offsetFromTableStart }{}
		};

		//----------------------------------------------------------------------------------------------------//
		//---------- DX12_RootParameter ----------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//

		class DX12_RootParameter : public D3D12_ROOT_PARAMETER1{
		public:
			constexpr void InitAsConstants(uint32_t numConstants_, D3D12_SHADER_VISIBILITY visibility_, uint32_t shaderRegister_, uint32_t space_ = 0){
				ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
				ShaderVisibility = visibility_;
				Constants.Num32BitValues = numConstants_;
				Constants.ShaderRegister = shaderRegister_;
				Constants.RegisterSpace = space_;
			}

			constexpr void InitAsCBV(D3D12_SHADER_VISIBILITY visibility_, uint32_t shaderRegister_, uint32_t space_ = 0, D3D12_ROOT_DESCRIPTOR_FLAGS flags_ = D3D12_ROOT_DESCRIPTOR_FLAG_NONE){
				InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE_CBV, visibility_, shaderRegister_, space_, flags_);
			}

			constexpr void InitAsSRV(D3D12_SHADER_VISIBILITY visibility_, uint32_t shaderRegister_, uint32_t space_ = 0, D3D12_ROOT_DESCRIPTOR_FLAGS flags_ = D3D12_ROOT_DESCRIPTOR_FLAG_NONE){
				InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE_SRV, visibility_, shaderRegister_, space_, flags_);
			}

			constexpr void InitAsUAV(D3D12_SHADER_VISIBILITY visibility_, uint32_t shaderRegister_, uint32_t space_ = 0, D3D12_ROOT_DESCRIPTOR_FLAGS flags_ = D3D12_ROOT_DESCRIPTOR_FLAG_NONE){
				InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE_UAV, visibility_, shaderRegister_, space_, flags_);
			}

			constexpr void InitAsTable(D3D12_SHADER_VISIBILITY visibility_, const DX12_DescriptorRange* ranges_, uint32_t rangeCount_){
				static_assert(sizeof(DX12_DescriptorRange) == sizeof(D3D12_DESCRIPTOR_RANGE1));

				ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				ShaderVisibility = visibility_;
				DescriptorTable.NumDescriptorRanges = rangeCount_;
				DescriptorTable.pDescriptorRanges = ranges_;
			}

		private:
			constexpr void InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE type_, D3D12_SHADER_VISIBILITY visibility_, uint32_t shaderRegister_, uint32_t space_, D3D12_ROOT_DESCRIPTOR_FLAGS flags_){
				ParameterType = type_;
				ShaderVisibility = visibility_;
				Descriptor.ShaderRegister = shaderRegister_;
				Descriptor.RegisterSpace = space_;
				Descriptor.Flags = flags_;
			}
		};

		//----------------------------------------------------------------------------------------------------//
		//---------- DX12_RootSignatureDesc ------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//

		struct DX12_RootSignatureDesc : public D3D12_ROOT_SIGNATURE_DESC1{
			constexpr explicit DX12_RootSignatureDesc(const DX12_RootParameter* params_, uint32_t paramCount_,
				const D3D12_STATIC_SAMPLER_DESC* samplers_ = nullptr,
				uint32_t samplerCount_ = 0, D3D12_ROOT_SIGNATURE_FLAGS flags_ =
				D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS
			) : D3D12_ROOT_SIGNATURE_DESC1{ paramCount_, params_, samplerCount_, samplers_, flags_ }{}

			ID3D12RootSignature* Create(ID3D12Device* device_) const;
		};

		//----------------------------------------------------------------------------------------------------//
		//---------- DX12_PipelineStateSubObject -------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//

		template<D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type_, typename T>
		class alignas(void*) DX12_PipelineStateSubObject{
		public:
			DX12_PipelineStateSubObject() : type(type_), subObject(){}
			constexpr explicit DX12_PipelineStateSubObject(const T& subObject_) : type(type_), subObject(subObject_){}
			DX12_PipelineStateSubObject& operator=(const T& subObject_){ subObject = subObject_; return *this; }

		private:
			const D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type;
			T subObject;
		};

		#define DX12_HELPER_PSSO(name, ...) using DX12_PipelineStateSubObject_##name = DX12_PipelineStateSubObject<__VA_ARGS__>;

		DX12_HELPER_PSSO(RootSignature, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_ROOT_SIGNATURE, ID3D12RootSignature*);
		DX12_HELPER_PSSO(VS, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VS, D3D12_SHADER_BYTECODE);
		DX12_HELPER_PSSO(PS, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PS, D3D12_SHADER_BYTECODE);
		DX12_HELPER_PSSO(DS, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DS, D3D12_SHADER_BYTECODE);
		DX12_HELPER_PSSO(HS, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_HS, D3D12_SHADER_BYTECODE);
		DX12_HELPER_PSSO(GS, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_GS, D3D12_SHADER_BYTECODE);
		DX12_HELPER_PSSO(CS, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CS, D3D12_SHADER_BYTECODE);
		DX12_HELPER_PSSO(StreamOutput, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_STREAM_OUTPUT, D3D12_STREAM_OUTPUT_DESC);
		DX12_HELPER_PSSO(Blend, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_BLEND, D3D12_BLEND_DESC);
		DX12_HELPER_PSSO(SampleMask, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_MASK, uint32_t);
		DX12_HELPER_PSSO(Rasterizer, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RASTERIZER, D3D12_RASTERIZER_DESC);
		DX12_HELPER_PSSO(DepthStencil, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL, D3D12_DEPTH_STENCIL_DESC);
		DX12_HELPER_PSSO(InputLayout, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_INPUT_LAYOUT, D3D12_INPUT_LAYOUT_DESC);
		DX12_HELPER_PSSO(IBStripCutValue, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_IB_STRIP_CUT_VALUE, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE);
		DX12_HELPER_PSSO(PrimitiveTopology, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PRIMITIVE_TOPOLOGY, D3D12_PRIMITIVE_TOPOLOGY_TYPE);
		DX12_HELPER_PSSO(RenderTargetFormats, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS, D3D12_RT_FORMAT_ARRAY);
		DX12_HELPER_PSSO(DepthStencilFormat, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL_FORMAT, DXGI_FORMAT);
		DX12_HELPER_PSSO(SampleDesc, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_DESC, DXGI_SAMPLE_DESC);
		DX12_HELPER_PSSO(NodeMask, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_NODE_MASK, uint32_t);
		DX12_HELPER_PSSO(CachedPSO, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CACHED_PSO, D3D12_CACHED_PIPELINE_STATE);
		DX12_HELPER_PSSO(Flags, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_FLAGS, D3D12_PIPELINE_STATE_FLAGS);
		DX12_HELPER_PSSO(DepthStencil1, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL1, D3D12_DEPTH_STENCIL_DESC1);
		DX12_HELPER_PSSO(ViewInstancing, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VIEW_INSTANCING, D3D12_VIEW_INSTANCING_DESC);
		DX12_HELPER_PSSO(AS, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_AS, D3D12_SHADER_BYTECODE);
		DX12_HELPER_PSSO(MS, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_MS, D3D12_SHADER_BYTECODE);

		#undef DX12_HELPER_PSSO

		constexpr inline ID3D12PipelineState* CreatePipelineState(ID3D12Device8* device_, const D3D12_PIPELINE_STATE_STREAM_DESC& desc_){
			GADGET_BASIC_ASSERT(device_ != nullptr);
			GADGET_BASIC_ASSERT(desc_.pPipelineStateSubobjectStream != nullptr);
			GADGET_BASIC_ASSERT(desc_.SizeInBytes > 0);

			ID3D12PipelineState* pso = nullptr;
			HRESULT result = device_->CreatePipelineState(&desc_, IID_PPV_ARGS(&pso));
			if(FAILED(result) || pso == nullptr){
				Debug::Log(SID("RENDER"), "Error!", Debug::Error, __FILE__, __LINE__);
				pso = nullptr; //I don't think this can fail in a way that results in this NOT being nullptr, but might as well play it safe
			}

			return pso;
		}

		constexpr inline ID3D12PipelineState* CreatePipelineState(ID3D12Device8* device_, void* stream_, uint64_t size_){
			GADGET_BASIC_ASSERT(device_ != nullptr);
			GADGET_BASIC_ASSERT(stream_ != nullptr);
			GADGET_BASIC_ASSERT(size_ > 0);

			D3D12_PIPELINE_STATE_STREAM_DESC desc{};
			desc.SizeInBytes = size_;
			desc.pPipelineStateSubobjectStream = stream_;
			return CreatePipelineState(device_, desc);
		}
	}
}

#endif //!GADGET_DX12_HELPERS_H
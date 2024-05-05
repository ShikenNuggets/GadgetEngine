#ifndef GADGET_DX12_HELPERS_H
#define GADGET_DX12_HELPERS_H

#include <cstdint>

#include <dxgi1_6.h>
#include <d3d12.h>

#include "Debug.h"
#include "Graphics/DX12/DX12_Defines.h"
#include "Graphics/DX12/DX12_UploadContext.h"

namespace Gadget{
	//Some of these are loosely based on Microsoft's d3dx12 helper library: https://github.com/microsoft/DirectX-Headers/blob/main/include/directx/d3dx12.h
	//TODO - I wanted to implement these myself while I'm learning how DX12 works, but we should consider using the "official" helpers
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

		constexpr D3D12_HEAP_PROPERTIES UploadHeapProperties{
			D3D12_HEAP_TYPE_UPLOAD,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			1,	//CreationNodeMask
			1	//VisibleNodeMask
		};

		constexpr D3D12_RASTERIZER_DESC RasterizerNoCulling{
			D3D12_FILL_MODE_SOLID,
			D3D12_CULL_MODE_NONE,
			0,
			0,
			0,
			0,
			1, //DepthClipEnable
			1, //MultisampleEnable
			0,
			0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		};

		constexpr D3D12_RASTERIZER_DESC RasterizerBackFaceCulling{
			D3D12_FILL_MODE_SOLID,
			D3D12_CULL_MODE_BACK,
			0,
			0,
			0,
			0,
			1, //DepthClipEnable
			1, //MultisampleEnable
			0,
			0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		};

		constexpr D3D12_RASTERIZER_DESC RasterizerFrontFaceCulling{
			D3D12_FILL_MODE_SOLID,
			D3D12_CULL_MODE_FRONT,
			0,
			0,
			0,
			0,
			1, //DepthClipEnable
			1, //MultisampleEnable
			0,
			0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		};

		constexpr D3D12_RASTERIZER_DESC RasterizerWireframe{
			D3D12_FILL_MODE_WIREFRAME,
			D3D12_CULL_MODE_NONE,
			0,
			0,
			0,
			0,
			1, //DepthClipEnable
			1, //MultisampleEnable
			0,
			0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		};

		constexpr D3D12_DEPTH_STENCIL_DESC1 DepthStencilDisabled{
			0, //DepthEnable
			D3D12_DEPTH_WRITE_MASK_ZERO,
			D3D12_COMPARISON_FUNC_LESS_EQUAL,
			0,
			0,
			0,
			{},
			{},
			0
		};

		constexpr D3D12_DEPTH_STENCIL_DESC1 DepthStencilEnabled{
			1, //DepthEnable
			D3D12_DEPTH_WRITE_MASK_ALL,
			D3D12_COMPARISON_FUNC_LESS_EQUAL,
			0,
			0,
			0,
			{},
			{},
			0
		};

		constexpr D3D12_DEPTH_STENCIL_DESC1 DepthStencilEnabledReadOnly{
			1, //DepthEnable
			D3D12_DEPTH_WRITE_MASK_ZERO,
			D3D12_COMPARISON_FUNC_LESS_EQUAL,
			0,
			0,
			0,
			{},
			{},
			0
		};

		constexpr D3D12_DEPTH_STENCIL_DESC1 DepthStencilReversed{
			1, //DepthEnable
			D3D12_DEPTH_WRITE_MASK_ALL,
			D3D12_COMPARISON_FUNC_GREATER_EQUAL,
			0,
			0,
			0,
			{},
			{},
			0
		};

		constexpr D3D12_DEPTH_STENCIL_DESC1 DepthStencilReversedReadOnly{
			1, //DepthEnable
			D3D12_DEPTH_WRITE_MASK_ZERO,
			D3D12_COMPARISON_FUNC_GREATER_EQUAL,
			0,
			0,
			0,
			{},
			{},
			0
		};

		constexpr D3D12_BLEND_DESC BlendDisabled{
			0,										//AlphaToCoverageEnable
			0,										//IndependentBlendEnable
			{
				{
					0,								//BlendEnable
					0,								//LogicOpEnable
					D3D12_BLEND_SRC_ALPHA,			//SrcBlend
					D3D12_BLEND_INV_SRC_ALPHA,		//DestBlend
					D3D12_BLEND_OP_ADD,				//BlendOp
					D3D12_BLEND_ONE,				//SrcBlendAlpha
					D3D12_BLEND_INV_SRC_ALPHA,		//DestBlendAlpha
					D3D12_BLEND_OP_ADD,				//BlendOpAlpha
					D3D12_LOGIC_OP_NOOP,			//LogicOp
					D3D12_COLOR_WRITE_ENABLE_ALL	//RenderTargetWriteMask
				},
				{},{},{},{},{},{},{}
			}
		};

		constexpr D3D12_BLEND_DESC BlendAlpha{
			0,										//AlphaToCoverageEnable
			0,										//IndependentBlendEnable
			{
				{
					1,								//BlendEnable
					0,								//LogicOpEnable
					D3D12_BLEND_SRC_ALPHA,			//SrcBlend
					D3D12_BLEND_INV_SRC_ALPHA,		//DestBlend
					D3D12_BLEND_OP_ADD,				//BlendOp
					D3D12_BLEND_ONE,				//SrcBlendAlpha
					D3D12_BLEND_ONE,				//DestBlendAlpha
					D3D12_BLEND_OP_ADD,				//BlendOpAlpha
					D3D12_LOGIC_OP_NOOP,			//LogicOp
					D3D12_COLOR_WRITE_ENABLE_ALL	//RenderTargetWriteMask
				},
				{},{},{},{},{},{},{}
			}
		};

		constexpr D3D12_BLEND_DESC BlendAdditive{
			0,										//AlphaToCoverageEnable
			0,										//IndependentBlendEnable
			{
				{
					1,								//BlendEnable
					0,								//LogicOpEnable
					D3D12_BLEND_ONE,				//SrcBlend
					D3D12_BLEND_ONE,				//DestBlend
					D3D12_BLEND_OP_ADD,				//BlendOp
					D3D12_BLEND_ONE,				//SrcBlendAlpha
					D3D12_BLEND_ONE,				//DestBlendAlpha
					D3D12_BLEND_OP_ADD,				//BlendOpAlpha
					D3D12_LOGIC_OP_NOOP,			//LogicOp
					D3D12_COLOR_WRITE_ENABLE_ALL	//RenderTargetWriteMask
				},
				{},{},{},{},{},{},{}
			}
		};

		constexpr D3D12_BLEND_DESC BlendPreMultiplied{
			0,										//AlphaToCoverageEnable
			0,										//IndependentBlendEnable
			{
				{
					0,								//BlendEnable
					0,								//LogicOpEnable
					D3D12_BLEND_ONE,				//SrcBlend
					D3D12_BLEND_INV_SRC_ALPHA,		//DestBlend
					D3D12_BLEND_OP_ADD,				//BlendOp
					D3D12_BLEND_ONE,				//SrcBlendAlpha
					D3D12_BLEND_ONE,				//DestBlendAlpha
					D3D12_BLEND_OP_ADD,				//BlendOpAlpha
					D3D12_LOGIC_OP_NOOP,			//LogicOp
					D3D12_COLOR_WRITE_ENABLE_ALL	//RenderTargetWriteMask
				},
				{},{},{},{},{},{},{}
			}
		};

		//----------------------------------------------------------------------------------------------------//
		//---------- DX12_DescriptorRange --------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//

		struct DX12_DescriptorRange : public D3D12_DESCRIPTOR_RANGE{
			constexpr explicit DX12_DescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE rangeType_,
				uint32_t descriptorCount_, uint32_t shaderRegister_, uint32_t space_ = 0,
				/*D3D12_DESCRIPTOR_RANGE_FLAGS flags_ = D3D12_DESCRIPTOR_RANGE_FLAG_NONE,*/
				uint32_t offsetFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
			) : D3D12_DESCRIPTOR_RANGE{ rangeType_, descriptorCount_, shaderRegister_, space_, /*flags_,*/ offsetFromTableStart }{}
		};

		//----------------------------------------------------------------------------------------------------//
		//---------- DX12_RootParameter ----------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//

		class DX12_RootParameter : public D3D12_ROOT_PARAMETER{
		public:
			constexpr void InitAsConstants(uint32_t numConstants_, D3D12_SHADER_VISIBILITY visibility_, uint32_t shaderRegister_, uint32_t space_ = 0){
				ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
				ShaderVisibility = visibility_;
				Constants.Num32BitValues = numConstants_;
				Constants.ShaderRegister = shaderRegister_;
				Constants.RegisterSpace = space_;
			}

			constexpr void InitAsCBV(D3D12_SHADER_VISIBILITY visibility_, uint32_t shaderRegister_, uint32_t space_ = 0){
				InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE_CBV, visibility_, shaderRegister_, space_);
			}

			constexpr void InitAsSRV(D3D12_SHADER_VISIBILITY visibility_, uint32_t shaderRegister_, uint32_t space_ = 0){
				InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE_SRV, visibility_, shaderRegister_, space_);
			}

			constexpr void InitAsUAV(D3D12_SHADER_VISIBILITY visibility_, uint32_t shaderRegister_, uint32_t space_ = 0){
				InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE_UAV, visibility_, shaderRegister_, space_);
			}

			constexpr void InitAsTable(D3D12_SHADER_VISIBILITY visibility_, const DX12_DescriptorRange* ranges_, uint32_t rangeCount_){
				static_assert(sizeof(DX12_DescriptorRange) == sizeof(D3D12_DESCRIPTOR_RANGE));

				ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				ShaderVisibility = visibility_;
				DescriptorTable.NumDescriptorRanges = rangeCount_;
				DescriptorTable.pDescriptorRanges = ranges_;
			}

		private:
			constexpr void InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE type_, D3D12_SHADER_VISIBILITY visibility_, uint32_t shaderRegister_, uint32_t space_){
				ParameterType = type_;
				ShaderVisibility = visibility_;
				Descriptor.ShaderRegister = shaderRegister_;
				Descriptor.RegisterSpace = space_;
			}
		};

		//----------------------------------------------------------------------------------------------------//
		//---------- DX12_RootSignatureDesc ------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//

		struct DX12_RootSignatureDesc : public D3D12_ROOT_SIGNATURE_DESC{
			constexpr static D3D12_ROOT_SIGNATURE_FLAGS DefaultFlags =
				D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS
				| D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED
				| D3D12_ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED
			;

			constexpr explicit DX12_RootSignatureDesc(const DX12_RootParameter* params_, uint32_t paramCount_,
				D3D12_ROOT_SIGNATURE_FLAGS flags_ = DefaultFlags,
				const D3D12_STATIC_SAMPLER_DESC* samplers_ = nullptr,
				uint32_t samplerCount_ = 0
			) : D3D12_ROOT_SIGNATURE_DESC{ paramCount_, params_, samplerCount_, samplers_, flags_ }{}

			ID3D12RootSignature* Create(ID3D12Device* device_) const;
		};

		//----------------------------------------------------------------------------------------------------//
		//---------- DX12_PipelineStateSubObject -------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//

		#pragma warning(disable : 4324) //Structure was padded due to alignment specifier - Not a concern here

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

		#pragma warning(default : 4324)

		#undef DX12_HELPER_PSSO

		struct DX12_PipelineStateSubobjectStream{
			DX12_PipelineStateSubObject_RootSignature rootSignature{ nullptr };
			DX12_PipelineStateSubObject_VS vs{};
			DX12_PipelineStateSubObject_PS ps{};
			DX12_PipelineStateSubObject_DS ds{};
			DX12_PipelineStateSubObject_HS hs{};
			DX12_PipelineStateSubObject_GS gs{};
			DX12_PipelineStateSubObject_CS cs{};
			DX12_PipelineStateSubObject_StreamOutput		streamOutput{};
			DX12_PipelineStateSubObject_Blend				blend{ BlendDisabled };
			DX12_PipelineStateSubObject_SampleMask			sampleMask{ std::numeric_limits<UINT>::max() };
			DX12_PipelineStateSubObject_Rasterizer			rasterizer{ RasterizerNoCulling };
			DX12_PipelineStateSubObject_InputLayout			inputLayout{};
			DX12_PipelineStateSubObject_IBStripCutValue		ibStripCutValue{};
			DX12_PipelineStateSubObject_PrimitiveTopology	primitiveTopology{};
			DX12_PipelineStateSubObject_RenderTargetFormats	renderTargetFormats{};
			DX12_PipelineStateSubObject_DepthStencilFormat	depthStencilFormat{};
			DX12_PipelineStateSubObject_SampleDesc			sampleDesc{{ 1, 0 }};
			DX12_PipelineStateSubObject_NodeMask			nodeMask{};
			DX12_PipelineStateSubObject_CachedPSO			cachedPSO{};
			DX12_PipelineStateSubObject_Flags				flags{};
			DX12_PipelineStateSubObject_DepthStencil1		depthStencil1{ DepthStencilDisabled };
			DX12_PipelineStateSubObject_ViewInstancing		viewInstancing{};
			DX12_PipelineStateSubObject_AS					as{};
			DX12_PipelineStateSubObject_MS					ms{};
		};

		constexpr inline ID3D12PipelineState* CreatePipelineState(ID3D12Device2* device_, const D3D12_PIPELINE_STATE_STREAM_DESC& desc_){
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

		constexpr inline ID3D12PipelineState* CreatePipelineState(ID3D12Device2* device_, void* stream_, uint64_t size_){
			GADGET_BASIC_ASSERT(device_ != nullptr);
			GADGET_BASIC_ASSERT(stream_ != nullptr);
			GADGET_BASIC_ASSERT(size_ > 0);

			D3D12_PIPELINE_STATE_STREAM_DESC desc{};
			desc.SizeInBytes = size_;
			desc.pPipelineStateSubobjectStream = stream_;
			return CreatePipelineState(device_, desc);
		}

		//----------------------------------------------------------------------------------------------------//
		//-------------------- Resource Barriers -------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//

		class DX12_ResourceBarriers{
		public:
			void AddTransitionBarrier(	ID3D12Resource* resource_,
										D3D12_RESOURCE_STATES before_, D3D12_RESOURCE_STATES after_,
										D3D12_RESOURCE_BARRIER_FLAGS flags_ = D3D12_RESOURCE_BARRIER_FLAG_NONE,
										uint32_t subResource_ = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

			void AddUAVBarrier(ID3D12Resource* resource_, D3D12_RESOURCE_BARRIER_FLAGS flags_ = D3D12_RESOURCE_BARRIER_FLAG_NONE);
			void AddAliasingBarrier(ID3D12Resource* beforeResource_, ID3D12Resource* afterResource_, D3D12_RESOURCE_BARRIER_FLAGS flags_ = D3D12_RESOURCE_BARRIER_FLAG_NONE);

			void ApplyAllBarriers(ID3D12_GraphicsCommandList* cmdList_);

		private:
			std::vector<D3D12_RESOURCE_BARRIER> barriers;
			uint32_t offset = 0;
		};

		constexpr inline void TransitionResource(ID3D12_GraphicsCommandList* cmdList_, ID3D12Resource* resource_,
								D3D12_RESOURCE_STATES before_, D3D12_RESOURCE_STATES after_,
								D3D12_RESOURCE_BARRIER_FLAGS flags_ = D3D12_RESOURCE_BARRIER_FLAG_NONE,
								uint32_t subResource_ = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES){
			GADGET_BASIC_ASSERT(cmdList_ != nullptr);
			GADGET_BASIC_ASSERT(resource_ != nullptr);
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = flags_;
			barrier.Transition.pResource = resource_;
			barrier.Transition.StateBefore = before_;
			barrier.Transition.StateAfter = after_;
			barrier.Transition.Subresource = subResource_;

			cmdList_->ResourceBarrier(1, &barrier);
		}

		//----------------------------------------------------------------------------------------------------//
		//-------------------- Buffers -----------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//

		ID3D12Resource* CreateBuffer(ID3D12_Device* device_, const void* data_, uint64_t bufferSize_, bool isCpuAccessible_ = false,
									 D3D12_RESOURCE_STATES state_ = D3D12_RESOURCE_STATE_COMMON,
									 D3D12_RESOURCE_FLAGS flags_ = D3D12_RESOURCE_FLAG_NONE,
									 ID3D12Heap* heap_ = nullptr, uint64_t heapOffset_ = 0);

		void UpdateBuffer(ID3D12_Resource* buffer_, const void* data_, uint64_t bufferSize_, UINT subResource_ = 0);
	}
}

#endif //!GADGET_DX12_HELPERS_H
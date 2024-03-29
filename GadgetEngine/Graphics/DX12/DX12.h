#ifndef GADGET_DX12_H
#define GADGET_DX12_H

#include <mutex>

#include <dxgi1_6.h>
#include <d3d12.h>

#include "Debug.h"
#include "Utils/Utils.h"

namespace Gadget{
	class DX12_Command;
	class DX12_DescriptorHeap;
	class DX12_RenderSurface;

	class DX12{
	public:
		DISABLE_COPY_AND_MOVE(DX12)

		DX12(){ GADGET_BASIC_ASSERT(!IsInitialized()); }

		static bool IsInitialized();
		static ID3D12Device8* const MainDevice();
		static uint32_t CurrentFrameIndex();

		static void DeferredRelease(IUnknown* resource_);
		static void SetDeferredReleaseFlag();
		static void ProcessDeferredReleases(uint32_t frameIndex_);
		static void ProcessAllDeferredReleases(); //ONLY USE THIS ON SHUTDOWN

		static DX12_DescriptorHeap& RTVHeap(){ return rtvDescriptorHeap; }
		static DX12_DescriptorHeap& DSVHeap(){ return dsvDescriptorHeap; }
		static DX12_DescriptorHeap& SRVHeap(){ return srvDescriptorHeap; }
		static DX12_DescriptorHeap& UAVHeap(){ return uavDescriptorHeap; }

		static void CreateSwapChainForSurface(DX12_RenderSurface* surface_);
		static void ResizeSurface(DX12_RenderSurface* surface_, int width_, int height_);

		static constexpr uint32_t FrameBufferCount = 3;
		static constexpr DXGI_FORMAT DefaultRenderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	protected:
		static IDXGIFactory7* dxgiFactory;
		static ID3D12Device8* mainDevice;
		static DX12_Command* gfxCommand;
		static DX12_DescriptorHeap rtvDescriptorHeap;
		static DX12_DescriptorHeap dsvDescriptorHeap;
		static DX12_DescriptorHeap srvDescriptorHeap;
		static DX12_DescriptorHeap uavDescriptorHeap;
		static std::vector<IUnknown*> deferredReleases[FrameBufferCount];
		static uint32_t deferredReleaseFlag[FrameBufferCount];
		static std::mutex deferredReleaseMutex;
	};

	namespace DX12_Helpers{
		constexpr D3D12_HEAP_PROPERTIES DefaultHeapProperties{
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			0,	//CreationNodeMask
			0	//VisibleNodeMask
		};

		struct DX12_DescriptorRange : public D3D12_DESCRIPTOR_RANGE1{
			constexpr explicit DX12_DescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE rangeType_,
													uint32_t descriptorCount_, uint32_t shaderRegister_, uint32_t space_ = 0,
													D3D12_DESCRIPTOR_RANGE_FLAGS flags_ = D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
													uint32_t offsetFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
			) : D3D12_DESCRIPTOR_RANGE1{ rangeType_, descriptorCount_, shaderRegister_, space_, flags_, offsetFromTableStart }{}
		};

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
	}
}

#endif //!GADGET_DX12_H
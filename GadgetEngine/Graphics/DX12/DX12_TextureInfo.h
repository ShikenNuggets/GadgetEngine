#ifndef GADGET_DX12_TEXTURE_INFO_H
#define GADGET_DX12_TEXTURE_INFO_H

#include "Graphics/TextureInfo.h"
#include "Graphics/DX12/DX12_DescriptorHeap.h"

namespace Gadget{
	struct DX12_TextureInitInfo{
		ID3D12Heap1* heap = nullptr;
		ID3D12Resource* resource = nullptr;
		D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr;
		D3D12_RESOURCE_DESC* resourceDesc = nullptr;
		D3D12_RESOURCE_ALLOCATION_INFO1 allocationInfo{};
		D3D12_RESOURCE_STATES initialState{};
		D3D12_CLEAR_VALUE clearValue{};
	};

	class DX12_TextureInfo : public TextureInfo{
	public:
		constexpr static uint32_t maxMips = 14;

		DX12_TextureInfo();
		explicit DX12_TextureInfo(const DX12_TextureInitInfo& info_);
		DISABLE_COPY_AND_MOVE(DX12_TextureInfo);
		virtual ~DX12_TextureInfo() override;

		void Initialize(const DX12_TextureInitInfo& info_);

		virtual void Bind([[maybe_unused]] int textureIndex_ = 0) override{ GADGET_ASSERT_NOT_IMPLEMENTED; }
		virtual void Unbind() override{ GADGET_ASSERT_NOT_IMPLEMENTED; }

		constexpr ID3D12Resource* const GetResource() const{ return resource; }
		constexpr DX12_DescriptorHandle SRV() const{ return srvHandle; }

	protected:
		ID3D12Resource* resource;
		DX12_DescriptorHandle srvHandle;
	};

	class DX12_RenderTextureInfo : public DX12_TextureInfo{
	public:
		DX12_RenderTextureInfo();
		explicit DX12_RenderTextureInfo(const DX12_TextureInitInfo& info_);
		DISABLE_COPY_AND_MOVE(DX12_RenderTextureInfo);
		virtual ~DX12_RenderTextureInfo() override;

		constexpr uint32_t MipCount() const{ return mipCount; }
		D3D12_CPU_DESCRIPTOR_HANDLE RTV(uint32_t mipIndex_) const;

	protected:
		DX12_DescriptorHandle rtvHandles[maxMips];
		uint32_t mipCount;
	};

	class DX12_DepthBufferTextureInfo : public DX12_TextureInfo{
	public:
		DX12_DepthBufferTextureInfo();
		explicit DX12_DepthBufferTextureInfo(DX12_TextureInitInfo info_);
		DISABLE_COPY_AND_MOVE(DX12_DepthBufferTextureInfo);
		virtual ~DX12_DepthBufferTextureInfo() override;

		constexpr D3D12_CPU_DESCRIPTOR_HANDLE DSV() const{ return dsvHandle.cpuHandle; }

	protected:
		DX12_DescriptorHandle dsvHandle;
	};
}

#endif //!GADGET_DX12_TEXTURE_INFO_H
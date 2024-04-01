#ifndef GADGET_DX12_GEOMETRY_PASS_H
#define GADGET_DX12_GEOMETRY_PASS_H

#include <d3d12.h>

#include "DX12.h"
#include "DX12_TextureInfo.h"
#include "ScreenCoordinate.h"
#include "Graphics/Color.h"
#include "Utils/Utils.h"

namespace Gadget{
	struct DX12_FrameInfo;

	class DX12_GeometryPass{
	public:
		STATIC_CLASS(DX12_GeometryPass);

		static bool Initialize(const ScreenCoordinate& size_, const Color& clearColor_ = Color::Black());
		static void Shutdown();

		static void SetClearColor(const Color& color_);
		static void OnResize(const ScreenCoordinate& newSize_);

		static void DepthPrepass(ID3D12_GraphicsCommandList* cmdList_, const DX12_FrameInfo& frameInfo_);
		static void Render(ID3D12_GraphicsCommandList* cmdList_, const DX12_FrameInfo& frameInfo_);

	private:
		constexpr static DXGI_FORMAT mainBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
		constexpr static DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;

		static DX12_RenderTextureInfo* mainBuffer;
		static DX12_DepthBufferTextureInfo* depthBuffer;
		static ID3D12RootSignature* rootSignature;
		static ID3D12PipelineState* pipelineStateObject;
		static ScreenCoordinate size;
		static Color clearColor;

		static bool CreateBuffers(const ScreenCoordinate& size_, const Color& clearColor_);
		static bool CreateRootSignatureAndPSO();
	};
}

#endif //!GADGET_DX12_GEOMETRY_PASS_H
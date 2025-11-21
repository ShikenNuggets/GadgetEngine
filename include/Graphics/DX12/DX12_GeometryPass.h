#ifndef GADGET_DX12_GEOMETRY_PASS_H
#define GADGET_DX12_GEOMETRY_PASS_H

#include <d3d12.h>

#include "GadgetEnums.h"
#include "ScreenCoordinate.h"
#include "Graphics/Color.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_TextureInfo.h"
#include "Utils/Utils.h"

namespace Gadget{
	class DX12_GeometryPass{
	public:
		STATIC_CLASS(DX12_GeometryPass);

		[[nodiscard]] static ErrorCode Initialize(const ScreenCoordinate& size_, const Color& clearColor_ = Color::Black());
		static void Shutdown();

		static bool IsInitialized();

		[[nodiscard]] static const DX12_RenderTextureInfo* MainBuffer(){ return mainBuffer; }
		[[nodiscard]] static const DX12_DepthBufferTextureInfo* DepthBuffer(){ return depthBuffer; }

		static void SetClearColor(const Color& color_);
		static void OnResize(const ScreenCoordinate& newSize_);

		static void DepthPrepass(ID3D12_GraphicsCommandList* cmdList_, const ScreenCoordinate& frameInfo_);
		static void Render(ID3D12_GraphicsCommandList* cmdList_, const ScreenCoordinate& frameInfo_);

		static void AddTransitionsForDepthPrepass(DX12_Helpers::DX12_ResourceBarriers& outResourceBarriers_);
		static void AddTransitionsForGeometryPrepass(DX12_Helpers::DX12_ResourceBarriers& outResourceBarriers_);
		static void AddTransitionsForPostProcess(DX12_Helpers::DX12_ResourceBarriers& outResourceBarriers_);

		static void SetRenderTargetsForDepthPrepass(ID3D12_GraphicsCommandList* cmdList_);
		static void SetRenderTargetsForGeometryPass(ID3D12_GraphicsCommandList* cmdList_);

	private:
		static constexpr DXGI_FORMAT mainBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
		static constexpr DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;

		static DX12_RenderTextureInfo* mainBuffer;
		static DX12_DepthBufferTextureInfo* depthBuffer;
		static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineStateObject;
		static ScreenCoordinate size;
		static Color clearColor;

		const static D3D12_RESOURCE_STATES initialMainBufferState;
		const static D3D12_RESOURCE_STATES initialDepthBufferState;

		static ErrorCode CreateBuffers(const ScreenCoordinate& size_, const Color& clearColor_);
		static ErrorCode CreateRootSignatureAndPSO();
	};
}

#endif //!GADGET_DX12_GEOMETRY_PASS_H
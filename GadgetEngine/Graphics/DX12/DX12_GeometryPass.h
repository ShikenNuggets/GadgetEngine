#ifndef GADGET_DX12_GEOMETRY_PASS_H
#define GADGET_DX12_GEOMETRY_PASS_H

#include <d3d12.h>

#include "DX12_TextureInfo.h"
#include "ScreenCoordinate.h"
#include "Utils/Utils.h"

namespace Gadget{
	class DX12_GeometryPass{
	public:
		STATIC_CLASS(DX12_GeometryPass);

		static bool Initialize();
		static void Shutdown();

		static void OnResize(const ScreenCoordinate& newSize_);

	private:
		constexpr static DXGI_FORMAT mainBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
		constexpr static DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;
		constexpr static ScreenCoordinate initialSize = ScreenCoordinate(100, 100);

		static DX12_RenderTextureInfo* mainBuffer;
		static DX12_DepthBufferTextureInfo* depthBuffer;
		static ScreenCoordinate size;

		static bool CreateBuffers(const ScreenCoordinate& size_);
	};
}

#endif //!GADGET_DX12_GEOMETRY_PASS_H
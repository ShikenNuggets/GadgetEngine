#ifndef GADGET_DX12_POST_PROCESS_H
#define GADGET_DX12_POST_PROCESS_H

#include <d3d12.h>
#include <wrl/client.h>

#include "Graphics/DX12/DX12_Defines.h"
#include "Graphics/DX12/DX12_TextureInfo.h"
#include "Utils/Utils.h"

namespace Gadget{
	class DX12_PostProcess{
	public:
		STATIC_CLASS(DX12_PostProcess);

		static bool Initialize();
		static void Shutdown();

		static void PostProcess(ID3D12_GraphicsCommandList* cmdList_, const DX12_RenderTextureInfo* geometryPassMainBuffer_, D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView_);

	private:
		static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		static Microsoft::WRL::ComPtr<ID3D12PipelineState> pso;

		static bool CreateRootSignatureAndPSO();
	};
}

#endif //!GADGET_DX12_POST_PROCESS_H
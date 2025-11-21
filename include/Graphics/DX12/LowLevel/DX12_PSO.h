#ifndef GADGET_DX12_PSO_H
#define GADGET_DX12_PSO_H

#include <cstdint>

#include <d3d12.h>

#include "Graphics/DX12/LowLevel/DX12_RootSignature.h"

namespace Gadget{
	class DX12_PSO{
	public:
		DX12_PSO(DXGI_FORMAT rtFormat_, DXGI_FORMAT depthFormat_, const DX12_RootSignature& signature_, const std::map<ShaderType::Type, D3D12_SHADER_BYTECODE>& shaders_);

	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pso;
	};
}

#endif //!GADGET_DX12_PSO_H
#ifndef GADGET_DX12_ROOT_SIGNATURE_H
#define GADGET_DX12_ROOT_SIGNATURE_H

#include <cstdint>

#include <wrl/client.h>

#include "Graphics/DX12/DX12_ShaderHandler.h"

namespace Gadget{
	struct OpaqueRootParameter{
		enum Parameter : uint32_t{
			PerFrameData,
			PositionBuffer,
			ElementBuffer,
			SrvIndices,
			PerObjectData,

			Parameter_MAX
		};
	};

	class DX12_RootSignature{
	public:
		DX12_RootSignature(const std::vector<ShaderType::Type>& types_);

	private:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

		static D3D12_ROOT_SIGNATURE_FLAGS GetRootSignatureFlags(const std::vector<ShaderType::Type>& types_);
	};
}

#endif //!GADGET_DX12_ROOT_SIGNATURE_H
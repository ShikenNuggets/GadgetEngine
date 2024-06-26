#ifndef GADGET_DX12_SHADER_HANDLER_H
#define GADGET_DX12_SHADER_HANDLER_H

#include <cstdint>

#include <d3d12.h>

#include "GadgetEnums.h"
#include "Resource/BinaryBlobResource.h"
#include "Utils/StringID.h"
#include "Utils/Utils.h"

namespace Gadget{
	struct ShaderType{ //Does this need to be a struct?
		enum class Type : uint32_t{
			Vertex = 0,
			Hull,
			Domain,
			Geometry,
			Pixel,
			Compute,
			Amplification,
			Mesh,

			Library,

			Type_MAX //Do not put anything below this!
		};
	};

	struct EngineShader{ //Does this need to be a struct?
		enum class ID : uint32_t{
			TestShader_VS = 0,
			TestShader_PS,
			PostProcess_PS,

			Hit_Lib,
			Miss_Lib,
			RayGen_Lib,

			ID_MAX //Do not put anything below this!
		};
	};

	struct CompiledShader{
		uint64_t size;
		const uint8_t* byteCode;
	};
	using CompiledShaderPtr = CompiledShader*;

	class DX12_ShaderHandler{
	public:
		STATIC_CLASS(DX12_ShaderHandler);

		[[nodiscard]] static ErrorCode Initialize();
		static void Shutdown();

		static D3D12_SHADER_BYTECODE GetEngineShader(EngineShader::ID id_);

	private:
		static const StringID shadersBlobResourceName;
		static BinaryBlobResource* shadersBlob;
		static CompiledShaderPtr engineShaders[(uint32_t)EngineShader::ID::ID_MAX];

		[[nodiscard]] static ErrorCode LoadEngineShaders();
	};
}

#endif //!GADGET_DX12_SHADER_HANDLER_H
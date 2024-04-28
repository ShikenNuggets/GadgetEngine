#ifndef DB_DX12_SHADER_COMPILER_H
#define DB_DX12_SHADER_COMPILER_H

#include <fstream>

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>

#include <Graphics/DX12/DX12_ShaderHandler.h>

namespace DB{
	using Shader = Microsoft::WRL::ComPtr<IDxcBlob>;
	using ShaderList = std::vector<Shader>;

	struct ShaderIncludeInfo{
		const char* file;
	};

	struct ShaderFileInfo{
		const char* file;
		const char* func;
		Gadget::EngineShader::ID id;
		Gadget::ShaderType::Type type;
	};

	constexpr const char* shaderSourcePath = "Build/Resources/";

	//This is just used for AreCompiledEngineShadersUpToDate check
	//The include handler figures out the actual inclusion logic on its own
	constexpr ShaderIncludeInfo shaderIncludes[]{
		{ "Common.hlsl" },
	};

	constexpr ShaderFileInfo shaderFiles[]{
		{ "TestShader.hlsl", "TestShaderVS", Gadget::EngineShader::ID::TestShader_VS, Gadget::ShaderType::Type::Vertex },
		{ "TestPixelShader.hlsl", "TestShaderPS", Gadget::EngineShader::ID::TestShader_PS, Gadget::ShaderType::Type::Pixel },
		{ "PostProcessPixelShader.hlsl", "PostProcessPS", Gadget::EngineShader::ID::PostProcess_PS, Gadget::ShaderType::Type::Pixel },

		{ "Hit.hlsl", "ClosestHit", Gadget::EngineShader::ID::Hit_Lib, Gadget::ShaderType::Type::Library },
		{ "Miss.hlsl", "Miss", Gadget::EngineShader::ID::Miss_Lib, Gadget::ShaderType::Type::Library },
		{ "RayGen.hlsl", "RayGen", Gadget::EngineShader::ID::RayGen_Lib, Gadget::ShaderType::Type::Library },
	};

	static_assert(_countof(shaderFiles) == (uint32_t)Gadget::EngineShader::ID::ID_MAX);

	class DX12_ShaderCompiler{
	public:
		DX12_ShaderCompiler();

		bool AreCompiledEngineShadersUpToDate(const std::string& outputFileName_);

		//Returns the number of shaders that were compiled
		//Returns -1 if an error occurs
		int CompileEngineShaders(const std::string& outputFileName_ = "EngineShaders.bin");

	private:
		IDxcBlob* Compile(IDxcBlobEncoding* sourceBlob_, LPCWSTR* args_, uint32_t numArgs_);

		IDxcBlob* Compile(const ShaderFileInfo& info_, const std::filesystem::path& fullPath_);

		bool SaveCompiledShaders(ShaderList& shaders_, const std::string& outputFileName_);

		constexpr static const char* profileStrings[]{ "vs_", "hs_", "ds_", "gs_", "ps_", "cs_", "as_", "ms_", "lib_"};
		static_assert(_countof(profileStrings) == (uint32_t)Gadget::ShaderType::Type::Type_MAX);

		const char* versionStr = "6_6";

		Microsoft::WRL::ComPtr<IDxcCompiler3> compiler;
		Microsoft::WRL::ComPtr<IDxcUtils> utils;
		Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;
	};
}

#endif //!DB_DX12_SHADER_COMPILER_H
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

	struct ShaderFileInfo{
		const char* file;
		const char* func;
		Gadget::EngineShader::ID id;
		Gadget::ShaderType::Type type;
	};

	//TODO - Hardcoding this here is obviously not ideal. This is just for testing!
	constexpr ShaderFileInfo shaderFiles[]{
		{ "TestShader.hlsl", "TestShaderVS", Gadget::EngineShader::ID::TestShader_VS, Gadget::ShaderType::Type::Vertex },
		{ "TestPixelShader.hlsl", "TestShaderPS", Gadget::EngineShader::ID::TestShader_PS, Gadget::ShaderType::Type::Pixel }
	};

	static_assert(_countof(shaderFiles) == (uint32_t)Gadget::EngineShader::ID::ID_MAX);
	constexpr const char* shaderSourcePath = "Build/Resources/";

	class DX12_ShaderCompiler{
	public:
		DX12_ShaderCompiler(){
			HRESULT hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
			if(FAILED(hr)){
				throw;
			}

			hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));
			if(FAILED(hr)){
				throw;
			}

			hr = utils->CreateDefaultIncludeHandler(&includeHandler);
			if(FAILED(hr)){
				throw;
			}
		}

		bool AreCompiledEngineShadersUpToDate(){ return false; } //TODO - Optimization!

		bool CompileEngineShaders(const std::string& outputFileName_ = "EngineShaders.bin"){
			if(AreCompiledEngineShadersUpToDate()){ return true; }

			ShaderList shaders;
			std::filesystem::path relPath{};
			std::filesystem::path absPath{};

			for(uint32_t i = 0; i < (uint32_t)Gadget::EngineShader::ID::ID_MAX; i++){
				auto& info = shaderFiles[i];

				relPath = shaderSourcePath;
				relPath += info.file;
				absPath = std::filesystem::absolute(relPath);
				if(!std::filesystem::exists(absPath)){
					return false;
				}

				Shader compiledShader = Compile(info, absPath);
				if(compiledShader != nullptr && compiledShader->GetBufferPointer() != nullptr && compiledShader->GetBufferSize() > 0){
					shaders.push_back(std::move(compiledShader));
				}else{
					return false;
				}
			}

			return SaveCompiledShaders(shaders, outputFileName_);
		}

	private:
		IDxcBlob* Compile(IDxcBlobEncoding* sourceBlob_, LPCWSTR* args_, uint32_t numArgs_){
			DxcBuffer buffer{};
			buffer.Encoding = DXC_CP_ACP;
			buffer.Ptr = sourceBlob_->GetBufferPointer();
			buffer.Size = sourceBlob_->GetBufferSize();

			Microsoft::WRL::ComPtr<IDxcResult> results = nullptr;
			HRESULT hr = compiler->Compile(&buffer, args_, numArgs_, includeHandler.Get(), IID_PPV_ARGS(&results));
			if(FAILED(hr) || results == nullptr){
				return nullptr;
			}

			Microsoft::WRL::ComPtr<IDxcBlobUtf8> errors = nullptr;
			hr = results->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
			if(FAILED(hr) || errors == nullptr){
				return nullptr;
			}

			if(errors != nullptr && errors->GetStringLength()){
				OutputDebugStringA("\nShader Compilation Error:\n");
				OutputDebugStringA(errors->GetStringPointer());
				OutputDebugStringA("\n");
			}

			HRESULT status = S_OK;
			hr = results->GetStatus(&status);
			if(FAILED(hr) || FAILED(status)){
				return nullptr;
			}

			Shader shader = nullptr;
			hr = results->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shader), nullptr);
			if(FAILED(hr) || shader == nullptr){
				return nullptr;
			}

			return shader.Detach();
		}

		IDxcBlob* Compile(const ShaderFileInfo& info_, const std::filesystem::path& fullPath_){
			assert(compiler != nullptr);
			assert(utils != nullptr);
			assert(includeHandler != nullptr);

			Microsoft::WRL::ComPtr<IDxcBlobEncoding> sourceBlob = nullptr;
			HRESULT hr = utils->LoadFile(fullPath_.c_str(), nullptr, &sourceBlob);
			if(FAILED(hr) || sourceBlob == nullptr){
				return nullptr;
			}

			assert(sourceBlob != nullptr);
			assert(sourceBlob->GetBufferPointer() != nullptr);
			assert(sourceBlob->GetBufferSize() != 0);

			std::wstring file = Gadget::Utils::ToWString(info_.file);
			std::wstring func = Gadget::Utils::ToWString(info_.func);
			std::wstring prof = Gadget::Utils::ToWString(std::string(profileStrings[(uint32_t)info_.type]) + versionStr);

			LPCWSTR args[]
			{
				file.c_str(),
				L"-E", func.c_str(),
				L"-T", prof.c_str(),
				DXC_ARG_ALL_RESOURCES_BOUND,

			#ifdef GADGET_DEBUG
				DXC_ARG_DEBUG,
				DXC_ARG_SKIP_OPTIMIZATIONS,
			#else
				DXC_ARG_OPTIMIZATION_LEVEL3,
			#endif

				DXC_ARG_WARNINGS_ARE_ERRORS,
				L"-Qstrip_reflect",
				L"-Qstrip_debug",
			};

			return Compile(sourceBlob.Get(), args, _countof(args));
		}

		bool SaveCompiledShaders(ShaderList& shaders_, const std::string& outputFileName_){
			const auto outputPath = std::filesystem::absolute(shaderSourcePath + outputFileName_);
			std::filesystem::create_directories(outputPath.parent_path());

			std::ofstream fileStream(outputPath, std::ios::out | std::ios::binary);
			if(!fileStream || !std::filesystem::exists(outputPath)){
				fileStream.close();
				return false;
			}

			for(const auto& shader : shaders_){
				const D3D12_SHADER_BYTECODE byteCode{ shader->GetBufferPointer(), shader->GetBufferSize() };
				fileStream.write((char*)&byteCode.BytecodeLength, sizeof(byteCode.BytecodeLength));
				fileStream.write((char*)&byteCode.pShaderBytecode, byteCode.BytecodeLength);
			}

			fileStream.close();
			return true;
		}

		const char* profileStrings[(uint32_t)Gadget::ShaderType::Type::Type_MAX]{ "vs_", "hs_", "ds_", "gs_", "ps_", "cs_", "as_", "ms_" };
		const char* versionStr = "6_5";

		Microsoft::WRL::ComPtr<IDxcCompiler3> compiler;
		Microsoft::WRL::ComPtr<IDxcUtils> utils;
		Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;
	};
}

#endif //!DB_DX12_SHADER_COMPILER_H
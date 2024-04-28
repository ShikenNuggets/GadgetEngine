#include "DX12_ShaderCompiler.h"

#include <iostream>

using namespace DB;

DX12_ShaderCompiler::DX12_ShaderCompiler(){
	HRESULT hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(compiler.ReleaseAndGetAddressOf()));
	if(FAILED(hr)){
		std::cout << "[DX12] ERROR: Could not create Dxc Compiler!" << std::endl;
		throw;
	}

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(utils.ReleaseAndGetAddressOf()));
	if(FAILED(hr)){
		std::cout << "[DX12] ERROR: Could not create Dxc Utils!" << std::endl;
		throw;
	}

	hr = utils->CreateDefaultIncludeHandler(includeHandler.ReleaseAndGetAddressOf());
	if(FAILED(hr)){
		std::cout << "[DX12] ERROR: Dxc Utils could not create the default include handler!" << std::endl;
		throw;
	}
}

bool DX12_ShaderCompiler::AreCompiledEngineShadersUpToDate(const std::string& outputFileName_){
	std::string outPath = shaderSourcePath + outputFileName_;
	if(!Gadget::FileSystem::FileExists(outPath)){
		return false; //Writing to the shader output for the first time
	}

	//Check last write time against includes
	for(int i = 0; i < std::size(shaderIncludes); i++){
		std::string incPath = std::string(shaderSourcePath) + shaderIncludes[i].file;
		_ASSERT(Gadget::FileSystem::FileExists(incPath));

		if(!Gadget::FileSystem::IsLastWriteTimeNewer(outPath, incPath)){
			return false;
		}
	}

	//Check last write time against engine shaders
	for(int i = 0; i < std::size(shaderFiles); i++){
		std::string inPath = std::string(shaderSourcePath) + shaderFiles[i].file;
		_ASSERT(Gadget::FileSystem::FileExists(inPath));

		if(!Gadget::FileSystem::IsLastWriteTimeNewer(outPath, inPath)){
			return false;
		}
	}

	return true;
}

//Returns the number of shaders that were compiled
//Returns -1 if an error occurs
int DX12_ShaderCompiler::CompileEngineShaders(const BuildOptions& options_, const std::string& outputFileName_){
	if(!options_.forceRebuild && AreCompiledEngineShadersUpToDate(outputFileName_)){
		std::cout << "[DX12] Existing compiled shaders are already up to date" << std::endl;
		return 0;
	}

	ShaderList shaders;
	std::filesystem::path relPath{};
	std::filesystem::path absPath{};

	for(uint32_t i = 0; i < (uint32_t)Gadget::EngineShader::ID::ID_MAX; i++){
		auto& info = shaderFiles[i];

		relPath = shaderSourcePath;
		relPath += info.file;
		absPath = std::filesystem::absolute(relPath);
		if(!std::filesystem::exists(absPath)){
			std::cout << "[DX12] ERROR: Shader source path [" + absPath.string() + "] does not exist!" << std::endl;
			return -1;
		}

		Shader compiledShader = Compile(info, absPath);
		if(compiledShader != nullptr && compiledShader->GetBufferPointer() != nullptr && compiledShader->GetBufferSize() > 0){
			shaders.push_back(std::move(compiledShader));
		} else{
			std::cout << "[DX12] ERROR: Could not compile shaders!" << std::endl;
			return -1;
		}
	}

	if(!SaveCompiledShaders(shaders, outputFileName_)){
		return -1;
	}

	return (int)shaders.size();
}

IDxcBlob* DX12_ShaderCompiler::Compile(IDxcBlobEncoding* sourceBlob_, LPCWSTR* args_, uint32_t numArgs_){
	DxcBuffer buffer{};
	buffer.Encoding = DXC_CP_ACP;
	buffer.Ptr = sourceBlob_->GetBufferPointer();
	buffer.Size = sourceBlob_->GetBufferSize();

	Microsoft::WRL::ComPtr<IDxcResult> results = nullptr;
	HRESULT hr = compiler->Compile(&buffer, args_, numArgs_, includeHandler.Get(), IID_PPV_ARGS(results.GetAddressOf()));
	if(FAILED(hr) || results == nullptr){
		std::cout << "[DX12] ERROR: IDxcCompiler3::Compile failed!" << std::endl;
		return nullptr;
	}

	Microsoft::WRL::ComPtr<IDxcBlobUtf8> errors = nullptr;
	hr = results->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(errors.GetAddressOf()), nullptr);
	if(FAILED(hr) || errors == nullptr){
		std::cout << "[DX12] ERROR: Could not get error blob from Dxc compilation!" << std::endl;
		return nullptr;
	}

	if(errors != nullptr && errors->GetStringLength()){
		std::cout << "[DX12] Shader Compilation Error: " << errors->GetStringPointer() << std::endl;
		OutputDebugStringA("\nShader Compilation Error:\n");
		OutputDebugStringA(errors->GetStringPointer());
		OutputDebugStringA("\n");
	}

	HRESULT status = S_OK;
	hr = results->GetStatus(&status);
	if(FAILED(hr) || FAILED(status)){
		std::cout << "[DX12] ERROR: Shader Compilation was not successful!" << std::endl;
		return nullptr;
	}

	Shader shader = nullptr;
	hr = results->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shader), nullptr);
	if(FAILED(hr) || shader == nullptr){
		std::cout << "[DX12] ERROR: Could not get shader compilation output!" << std::endl;
		return nullptr;
	}

	return shader.Detach();
}

IDxcBlob* DX12_ShaderCompiler::Compile(const ShaderFileInfo& info_, const std::filesystem::path& fullPath_){
	assert(compiler != nullptr);
	assert(utils != nullptr);
	assert(includeHandler != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlobEncoding> sourceBlob = nullptr;
	HRESULT hr = utils->LoadFile(fullPath_.c_str(), nullptr, &sourceBlob);
	if(FAILED(hr) || sourceBlob == nullptr){
		std::cout << "[DX12] ERROR: Could not load file [ " + fullPath_.string() + "]!" << std::endl;
		return nullptr;
	}

	assert(sourceBlob != nullptr);
	assert(sourceBlob->GetBufferPointer() != nullptr);
	assert(sourceBlob->GetBufferSize() != 0);

	std::wstring file = Gadget::Utils::ToWString(info_.file);
	std::wstring func = Gadget::Utils::ToWString(info_.func);
	std::wstring prof = Gadget::Utils::ToWString(std::string(profileStrings[(uint32_t)info_.type]) + versionStr);
	std::wstring incl = Gadget::Utils::ToWString(shaderSourcePath);

	LPCWSTR args[]
	{
		file.c_str(),
		L"-E ", func.c_str(),
		L"-T", prof.c_str(),
		L"-I", incl.c_str(),
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

bool DX12_ShaderCompiler::SaveCompiledShaders(ShaderList& shaders_, const std::string& outputFileName_){
	const auto outputPath = std::filesystem::absolute(shaderSourcePath + outputFileName_);
	std::filesystem::create_directories(outputPath.parent_path());

	std::ofstream fileStream(outputPath, std::ios::out | std::ios::binary);
	if(!fileStream || !std::filesystem::exists(outputPath)){
		std::cout << "[DX12] ERROR: Could not create filestream to output shader compilation result!" << std::endl;
		fileStream.close();
		return false;
	}

	for(const auto& shader : shaders_){
		const D3D12_SHADER_BYTECODE byteCode{ shader->GetBufferPointer(), shader->GetBufferSize() };

		_ASSERT(byteCode.BytecodeLength == shader->GetBufferSize());
		_ASSERT(byteCode.pShaderBytecode == shader->GetBufferPointer());

		fileStream.write((char*)&byteCode.BytecodeLength, sizeof(byteCode.BytecodeLength));
		fileStream.write((char*)byteCode.pShaderBytecode, byteCode.BytecodeLength);
	}

	fileStream.close();
	return true;
}
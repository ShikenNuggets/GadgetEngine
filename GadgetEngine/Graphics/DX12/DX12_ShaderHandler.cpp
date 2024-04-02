#include "DX12_ShaderHandler.h"

#include "App.h"

using namespace Gadget;

const StringID DX12_ShaderHandler::shadersBlobResourceName = SID("EngineShaders_DX12");
BinaryBlobResource* DX12_ShaderHandler::shadersBlob = nullptr;
CompiledShaderPtr DX12_ShaderHandler::engineShaders[(uint32_t)EngineShader::ID::ID_MAX]{};

bool DX12_ShaderHandler::Initialize(){
	return LoadEngineShaders();
}

void DX12_ShaderHandler::Shutdown(){
	for(uint32_t i = 0; i < (uint32_t)EngineShader::ID::ID_MAX; i++){
		engineShaders[i] = {};
	}
	
	if(shadersBlob != nullptr){
		App::GetResourceManager().UnloadResource(shadersBlobResourceName);
		shadersBlob = nullptr;
	}
}

D3D12_SHADER_BYTECODE DX12_ShaderHandler::GetEngineShader(EngineShader::ID id_){
	GADGET_BASIC_ASSERT(id_ < EngineShader::ID::ID_MAX);
	CompiledShaderPtr& shader = engineShaders[(uint32_t)id_];
	GADGET_BASIC_ASSERT(shader != nullptr);
	GADGET_BASIC_ASSERT(shader->byteCode != nullptr);
	GADGET_BASIC_ASSERT(shader->size > 0);

	return { shader->byteCode, shader->size };
}

bool DX12_ShaderHandler::LoadEngineShaders(){
	GADGET_BASIC_ASSERT(shadersBlob == nullptr);

	shadersBlob = App::GetResourceManager().LoadResource<BinaryBlobResource>(shadersBlobResourceName);

	GADGET_BASIC_ASSERT(shadersBlob != nullptr);
	GADGET_BASIC_ASSERT(shadersBlob->Size() != 0);

	uint64_t offset = 0;
	uint32_t index = 0;
	bool result = true;
	while(offset < shadersBlob->Size() && result == true){
		GADGET_BASIC_ASSERT(index < (uint32_t)EngineShader::ID::ID_MAX);
		CompiledShaderPtr& shader = engineShaders[index];
		GADGET_BASIC_ASSERT(shader == nullptr);
		result &= index < (uint32_t)EngineShader::ID::ID_MAX && !shader;

		if(!result){
			break;
		}

		shader = reinterpret_cast<CompiledShaderPtr>(&shadersBlob->Data().data()[offset]);
		offset += sizeof(uint64_t) + shader->size;
		index++;
	}

	GADGET_BASIC_ASSERT(offset == shadersBlob->Size() && index == (uint32_t)EngineShader::ID::ID_MAX);

	return result;
}
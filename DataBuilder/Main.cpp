#include <iostream>

#include "DX12_ShaderCompiler.h"

using namespace DB;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){
	std::cout << "Gadget Engine Data Builder - " << std::endl;

	std::cout << "[DX12] Compiling HLSL engine shaders..." << std::endl;
	DX12_ShaderCompiler compiler;
	bool result = compiler.CompileEngineShaders();
	if(result){
		std::cout << "[DX12] HLSL engine shaders compiled successfully!" << std::endl;
	}else{
		std::cout << "[DX12] HLSL engine shader compile failed!" << std::endl;
		return -1;
	}

	return 0;
}
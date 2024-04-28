#include <iostream>

#include "BuildOptions.h"
#include "DX12_ShaderCompiler.h"

using namespace DB;

int main(int argc, char* argv[]){
	std::cout << "Gadget Engine Data Builder - " << std::endl;

	//Set build options from arguments
	BuildOptions options = BuildOptions(argc, argv);

	//Compile HLSL shaders
	std::cout << "[DX12] Compiling HLSL engine shaders..." << std::endl;
	DX12_ShaderCompiler compiler;
	int result = compiler.CompileEngineShaders(options);
	if(result >= 0){
		std::cout << "[DX12] " << result << " HLSL engine shaders successfully compiled" << std::endl;
	}else{
		std::cout << "[DX12] HLSL engine shader compile failed!" << std::endl;
		return -1;
	}

	return 0;
}
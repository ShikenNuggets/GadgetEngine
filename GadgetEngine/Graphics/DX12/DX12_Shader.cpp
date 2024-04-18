#include "DX12_Shader.h"

using namespace Gadget;

DX12_Shader::DX12_Shader(const std::map<ShaderType::Type, D3D12_SHADER_BYTECODE>& shaders_) : 
						rootSignature(DX12_RootSignature(Utils::GetAllKeys(shaders_))),
						pso(DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_D32_FLOAT, rootSignature, shaders_){}
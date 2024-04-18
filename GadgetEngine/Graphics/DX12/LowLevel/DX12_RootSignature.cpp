#include "DX12_RootSignature.h"

#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Helpers.h"

using namespace Gadget;

DX12_RootSignature::DX12_RootSignature(const std::vector<ShaderType::Type>& types_) : rootSignature(){
	using params = OpaqueRootParameter;
	D3D12_SHADER_VISIBILITY bufferVisibility = D3D12_SHADER_VISIBILITY_ALL;
	D3D12_SHADER_VISIBILITY dataVisibility = D3D12_SHADER_VISIBILITY_ALL;
	D3D12_SHADER_VISIBILITY sampleVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	if(Utils::Contains(types_, ShaderType::Type::Vertex)){
		bufferVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		dataVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	}else if(Utils::Contains(types_, ShaderType::Type::Mesh)){
		bufferVisibility = D3D12_SHADER_VISIBILITY_MESH;
		dataVisibility = D3D12_SHADER_VISIBILITY_MESH;
	}

	DX12_Helpers::DX12_RootParameter parameters[params::Parameter_MAX]{};

	parameters[params::PositionBuffer].InitAsSRV(bufferVisibility, 0);
	parameters[params::ElementBuffer].InitAsSRV(bufferVisibility, 1);
	parameters[params::SrvIndices].InitAsSRV(sampleVisibility, 2);

	parameters[params::PerFrameData].InitAsCBV(D3D12_SHADER_VISIBILITY_ALL, 0);
	parameters[params::PerObjectData].InitAsCBV(dataVisibility, 1);

	auto rsDesc = DX12_Helpers::DX12_RootSignatureDesc(&parameters[0], static_cast<uint32_t>(std::size(parameters)), GetRootSignatureFlags(types_));
	rootSignature.Attach(rsDesc.Create(DX12::GetInstance().MainDevice()));
	GADGET_BASIC_ASSERT(rootSignature != nullptr);
}

D3D12_ROOT_SIGNATURE_FLAGS DX12_RootSignature::GetRootSignatureFlags(const std::vector<ShaderType::Type>& types_){
	D3D12_ROOT_SIGNATURE_FLAGS flags = DX12_Helpers::DX12_RootSignatureDesc::DefaultFlags;

	if(Utils::Contains(types_, ShaderType::Type::Vertex))			flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
	if(Utils::Contains(types_, ShaderType::Type::Hull))				flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
	if(Utils::Contains(types_, ShaderType::Type::Domain))			flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
	if(Utils::Contains(types_, ShaderType::Type::Geometry))			flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	if(Utils::Contains(types_, ShaderType::Type::Pixel))			flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
	if(Utils::Contains(types_, ShaderType::Type::Amplification))	flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS;
	if(Utils::Contains(types_, ShaderType::Type::Mesh))				flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS;

	return flags;
}
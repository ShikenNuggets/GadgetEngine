#include "DX12_PSO.h"

#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Helpers.h"

using namespace Gadget;

DX12_PSO::DX12_PSO(DXGI_FORMAT rtFormat_, DXGI_FORMAT depthFormat_, const DX12_RootSignature& signature_, const std::map<ShaderType::Type, D3D12_SHADER_BYTECODE>& shaders_){
	constexpr uint64_t alignedStreamSize(Utils::AlignSizeUp<sizeof(uint64_t)>(sizeof(DX12_Helpers::DX12_PipelineStateSubobjectStream)));
	uint8_t* const streamPtr = (uint8_t* const)_malloca(alignedStreamSize);
	GADGET_BASIC_ASSERT(streamPtr != nullptr);
	ZeroMemory(streamPtr, alignedStreamSize);
	new (streamPtr) DX12_Helpers::DX12_PipelineStateSubobjectStream{};

	DX12_Helpers::DX12_PipelineStateSubobjectStream& stream{ *(DX12_Helpers::DX12_PipelineStateSubobjectStream* const)streamPtr };

	D3D12_RT_FORMAT_ARRAY rtArray{};
	rtArray.NumRenderTargets = 1;
	rtArray.RTFormats[0] = rtFormat_;

	stream.renderTargetFormats = rtArray;
	stream.rootSignature = signature_.RootSignature();
	stream.primitiveTopology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	stream.depthStencilFormat = depthFormat_;
	stream.rasterizer = DX12_Helpers::RasterizerBackFaceCulling; //TODO - Different for PostProcess
	stream.depthStencil1 = DX12_Helpers::DepthStencilEnabledReadOnly; //TODO - Different for PostProcess
	stream.blend = DX12_Helpers::BlendDisabled;

	stream.vs = { nullptr };
	stream.hs = { nullptr };
	stream.ds = { nullptr };
	stream.gs = { nullptr };
	stream.ps = { nullptr };
	stream.cs = { nullptr };
	stream.as = { nullptr };
	stream.ms = { nullptr };

	for(const auto& s : shaders_){
		switch(s.first){
			case ShaderType::Type::Vertex:
				stream.vs = shaders_.at(s.first);
				break;
			case ShaderType::Type::Hull:
				stream.hs = shaders_.at(s.first);
				break;
			case ShaderType::Type::Domain:
				stream.ds = shaders_.at(s.first);
				break;
			case ShaderType::Type::Geometry:
				stream.gs = shaders_.at(s.first);
				break;
			case ShaderType::Type::Pixel:
				stream.ps = shaders_.at(s.first);
				break;
			case ShaderType::Type::Compute:
				stream.cs = shaders_.at(s.first);
				break;
			case ShaderType::Type::Amplification:
				stream.as = shaders_.at(s.first);
				break;
			case ShaderType::Type::Mesh:
				stream.ms = shaders_.at(s.first);
				break;
			default:
				GADGET_ASSERT_NOT_IMPLEMENTED;
				break;
		}
	}

	pso = DX12_Helpers::CreatePipelineState(DX12::GetInstance().MainDevice(), &stream, alignedStreamSize);
}
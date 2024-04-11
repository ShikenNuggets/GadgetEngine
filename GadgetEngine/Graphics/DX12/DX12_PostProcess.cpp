#include "DX12_PostProcess.h"

#include "Debug.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Helpers.h"
#include "Graphics/DX12/DX12_RenderSurface.h"
#include "Graphics/DX12/DX12_ShaderHandler.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

ComPtr<ID3D12RootSignature> DX12_PostProcess::rootSignature = nullptr;
ComPtr<ID3D12PipelineState> DX12_PostProcess::pso = nullptr;

enum PostProcessRootParamIndices : uint32_t{
	ID_RootConstants = 0,
	ID_DescriptorTable,

	ID_MAX //DO NOT PUT ANYTHING BELOW THIS!!!
};

ErrorCode DX12_PostProcess::Initialize(){
	return CreateRootSignatureAndPSO();
}

void DX12_PostProcess::Shutdown(){
	pso.Reset();
	rootSignature.Reset();
}

void DX12_PostProcess::PostProcess(ID3D12_GraphicsCommandList* cmdList_, const DX12_RenderTextureInfo* geometryPassMainBuffer_, D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView_){
	GADGET_BASIC_ASSERT(cmdList_ != nullptr);
	GADGET_BASIC_ASSERT(geometryPassMainBuffer_ != nullptr);
	GADGET_BASIC_ASSERT(rootSignature != nullptr);
	GADGET_BASIC_ASSERT(pso != nullptr);

	cmdList_->SetGraphicsRootSignature(rootSignature.Get());
	cmdList_->SetPipelineState(pso.Get());

	cmdList_->SetGraphicsRoot32BitConstant(ID_RootConstants, geometryPassMainBuffer_->SRV().GetIndex(), 0);
	cmdList_->SetGraphicsRootDescriptorTable(ID_DescriptorTable, DX12::GetInstance().SRVHeap().GPUStart());

	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList_->OMSetRenderTargets(1, &renderTargetView_, 1, nullptr);
	cmdList_->DrawInstanced(3, 1, 0, 0);
}

ErrorCode DX12_PostProcess::CreateRootSignatureAndPSO(){
	GADGET_BASIC_ASSERT(rootSignature == nullptr);
	GADGET_BASIC_ASSERT(pso == nullptr);

	DX12_Helpers::DX12_DescriptorRange range{
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND, 0, 0,
		D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE
	};

	//Root Signature
	DX12_Helpers::DX12_RootParameter rootParams[ID_MAX]{};
	rootParams[ID_RootConstants].InitAsConstants(1, D3D12_SHADER_VISIBILITY_PIXEL, 1);
	rootParams[ID_DescriptorTable].InitAsTable(D3D12_SHADER_VISIBILITY_PIXEL, &range, 1);
	DX12_Helpers::DX12_RootSignatureDesc rsDesc{ &rootParams[0], _countof(rootParams) };

	rootSignature.Attach(rsDesc.Create(DX12::GetInstance().MainDevice()));
	GADGET_BASIC_ASSERT(rootSignature != nullptr);
	if(rootSignature == nullptr){
		Debug::Log(SID("RENDER"), "PostProcess - Could not create root signature!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	rootSignature->SetName(L"PostProcess Root Signature");

	//Pipeline State Object
	struct{
		DX12_Helpers::DX12_PipelineStateSubObject_RootSignature			rootSignaturePSO{ rootSignature.Get()};
		DX12_Helpers::DX12_PipelineStateSubObject_VS					vs{ DX12_ShaderHandler::GetEngineShader(EngineShader::ID::TestShader_VS) };
		DX12_Helpers::DX12_PipelineStateSubObject_PS					ps{ DX12_ShaderHandler::GetEngineShader(EngineShader::ID::PostProcess_PS) };
		DX12_Helpers::DX12_PipelineStateSubObject_PrimitiveTopology		primitiveTopology{ D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE };
		DX12_Helpers::DX12_PipelineStateSubObject_RenderTargetFormats	renderTargetFormats;
		DX12_Helpers::DX12_PipelineStateSubObject_Rasterizer			rasterizer{ DX12_Helpers::RasterizerNoCulling };
	} psoStream;

	D3D12_RT_FORMAT_ARRAY rtfArray{};
	rtfArray.NumRenderTargets = 1;
	rtfArray.RTFormats[0] = DX12_RenderSurface::DefaultBackBufferFormat;
	psoStream.renderTargetFormats = rtfArray;

	pso.Attach(DX12_Helpers::CreatePipelineState(DX12::GetInstance().MainDevice(), &psoStream, sizeof(psoStream)));
	GADGET_BASIC_ASSERT(pso != nullptr);
	if(pso == nullptr){
		Debug::Log(SID("RENDER"), "PostProcess - Could not create pipeline state object!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	pso->SetName(L"PostProcess Pipeline State Object");

	return ErrorCode::OK;
}
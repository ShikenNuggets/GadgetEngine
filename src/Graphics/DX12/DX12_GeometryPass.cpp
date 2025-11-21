#include "DX12_GeometryPass.h"

#include "Graphics/DX12/DX12_Helpers.h"
#include "Graphics/DX12/DX12_MeshInfo.h"
#include "Graphics/DX12/DX12_ShaderHandler.h"

using namespace Gadget;

DX12_RenderTextureInfo* DX12_GeometryPass::mainBuffer = nullptr;
DX12_DepthBufferTextureInfo* DX12_GeometryPass::depthBuffer = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> DX12_GeometryPass::rootSignature = nullptr;
Microsoft::WRL::ComPtr<ID3D12PipelineState> DX12_GeometryPass::pipelineStateObject = nullptr;
ScreenCoordinate DX12_GeometryPass::size = ScreenCoordinate(0, 0);
Color DX12_GeometryPass::clearColor = Color::Black();

const D3D12_RESOURCE_STATES DX12_GeometryPass::initialMainBufferState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
const D3D12_RESOURCE_STATES DX12_GeometryPass::initialDepthBufferState = D3D12_RESOURCE_STATE_DEPTH_READ | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

ErrorCode DX12_GeometryPass::Initialize(const ScreenCoordinate& size_, const Color& clearColor_){
	GADGET_BASIC_ASSERT(size_.x > 0);
	GADGET_BASIC_ASSERT(size_.y > 0);
	GADGET_BASIC_ASSERT(mainBuffer == nullptr);
	GADGET_BASIC_ASSERT(depthBuffer == nullptr);

	if(IsInitialized()){
		GADGET_LOG_ERROR(SID("RENDER"), "Tried to re-initialize DX12_GeometryPass!");
		return ErrorCode::Invalid_State;
	}

	if(size_.x <= 0 || size_.y <= 0 || !clearColor.IsValid()){
		GADGET_LOG_ERROR(SID("RENDER"), "Initializing geometry pass with invalid parameters");
		return ErrorCode::Invalid_Args;
	}

	size = size_;
	clearColor = clearColor_;

	auto err = CreateBuffers(size, clearColor);
	if(err != ErrorCode::OK){
		Debug::Log(SID("RENDER"), "Could not create buffers for geometry pass!", Debug::Error, __FILE__, __LINE__);
		return err;
	}

	err = CreateRootSignatureAndPSO();
	if(err != ErrorCode::OK){
		Debug::Log(SID("RENDER"), "Could not create root signature and pipeline state object for geometry pass!", Debug::Error, __FILE__, __LINE__);
		return err;
	}

	return ErrorCode::OK;
}

void DX12_GeometryPass::Shutdown(){
	pipelineStateObject.Reset();
	rootSignature.Reset();

	if(mainBuffer != nullptr){
		delete mainBuffer;
		mainBuffer = nullptr;
	}

	if(depthBuffer != nullptr){
		delete depthBuffer;
		depthBuffer = nullptr;
	}

	size = ScreenCoordinate(0, 0);
	clearColor = Color::Black();
}

bool DX12_GeometryPass::IsInitialized(){ return mainBuffer != nullptr && depthBuffer != nullptr && rootSignature != nullptr && pipelineStateObject != nullptr; }

void DX12_GeometryPass::SetClearColor(const Color& color_){
	GADGET_BASIC_ASSERT(color_.IsValid());

	if(color_ == clearColor){
		return; //Clear color is the same - No need to do anything
	}

	clearColor = color_;
	CreateBuffers(size, color_); //This feels like it could be more efficient, but we generally only set the clear color once per session so it's probably fine
}

void DX12_GeometryPass::OnResize(const ScreenCoordinate& newSize_){
	GADGET_BASIC_ASSERT(newSize_.x > 0);
	GADGET_BASIC_ASSERT(newSize_.y > 0);

	if(newSize_ == size){
		return; //Size is the same - No need to do anything
	}

	size = newSize_;
	CreateBuffers(newSize_, clearColor);
}

void DX12_GeometryPass::DepthPrepass([[maybe_unused]] ID3D12_GraphicsCommandList* cmdList_, [[maybe_unused]] const ScreenCoordinate& frameInfo_){}

void DX12_GeometryPass::Render(ID3D12_GraphicsCommandList* cmdList_, [[maybe_unused]] const ScreenCoordinate& frameInfo_){
	GADGET_BASIC_ASSERT(frameInfo_.x >= 0);
	GADGET_BASIC_ASSERT(frameInfo_.y >= 0);

	cmdList_->SetGraphicsRootSignature(rootSignature.Get());
	cmdList_->SetPipelineState(pipelineStateObject.Get());

	//TODO - This is HIGHLY QUESTIONABLE. Basically this whole function is temporary just so I can test stuff while we develop the rest of the renderer
	static uint32_t frame = 0;
	frame++;
	cmdList_->SetGraphicsRoot32BitConstant(0, frame, 0);

	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList_->DrawInstanced(3, 1, 0, 0); //TODO - This just draws 1 triangle. More work needed for actual scene rendering
}

void DX12_GeometryPass::AddTransitionsForDepthPrepass(DX12_Helpers::DX12_ResourceBarriers& outResourceBarriers_){
	outResourceBarriers_.AddTransitionBarrier(depthBuffer->GetResource(), initialDepthBufferState, D3D12_RESOURCE_STATE_DEPTH_WRITE);
}

void DX12_GeometryPass::AddTransitionsForGeometryPrepass(DX12_Helpers::DX12_ResourceBarriers& outResourceBarriers_){
	outResourceBarriers_.AddTransitionBarrier(mainBuffer->GetResource(), initialMainBufferState, D3D12_RESOURCE_STATE_RENDER_TARGET);
	outResourceBarriers_.AddTransitionBarrier(depthBuffer->GetResource(), D3D12_RESOURCE_STATE_DEPTH_WRITE, initialDepthBufferState);
}

void DX12_GeometryPass::AddTransitionsForPostProcess(DX12_Helpers::DX12_ResourceBarriers& outResourceBarriers_){
	outResourceBarriers_.AddTransitionBarrier(mainBuffer->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, initialMainBufferState);
}

void DX12_GeometryPass::SetRenderTargetsForDepthPrepass(ID3D12_GraphicsCommandList* cmdList_){
	GADGET_BASIC_ASSERT(cmdList_ != nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE dsv = depthBuffer->DSV();
	cmdList_->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 0.0f, 0, 0, nullptr);
	cmdList_->OMSetRenderTargets(0, nullptr, 0, &dsv);
}

void DX12_GeometryPass::SetRenderTargetsForGeometryPass(ID3D12_GraphicsCommandList* cmdList_){
	GADGET_BASIC_ASSERT(cmdList_ != nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE rtv = mainBuffer->RTV(0);
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = depthBuffer->DSV();

	cmdList_->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
	cmdList_->OMSetRenderTargets(1, &rtv, 0, &dsv);
}

ErrorCode DX12_GeometryPass::CreateBuffers(const ScreenCoordinate& size_, const Color& clearColor_){
	GADGET_BASIC_ASSERT(size_.x > 0);
	GADGET_BASIC_ASSERT(size_.y > 0);
	GADGET_BASIC_ASSERT(clearColor_.IsValid());

	if(mainBuffer != nullptr){
		delete mainBuffer;
		mainBuffer = nullptr;
	}

	if(depthBuffer != nullptr){
		delete depthBuffer;
		depthBuffer = nullptr;
	}

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	desc.Format = mainBufferFormat;
	desc.Width = size_.x;
	desc.Height = size_.y;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.MipLevels = 0;
	desc.SampleDesc = { 1, 0 };

	//Main Buffer
	DX12_TextureInitInfo info;
	info.resourceDesc = &desc;
	info.initialState = initialMainBufferState;
	info.clearValue.Format = desc.Format;
	info.clearValue.Color[0] = clearColor_.r;
	info.clearValue.Color[1] = clearColor_.g;
	info.clearValue.Color[2] = clearColor_.b;
	info.clearValue.Color[3] = clearColor_.a;

	mainBuffer = new DX12_RenderTextureInfo(info);
	GADGET_BASIC_ASSERT(mainBuffer != nullptr);
	GADGET_BASIC_ASSERT(mainBuffer->GetResource() != nullptr);
	if(mainBuffer->GetResource() == nullptr){
		GADGET_LOG_ERROR(SID("RENDER"), "Couuld not create DX12_RenderTextureInfo for the main geometry buffer!");
		return ErrorCode::Constructor_Failed;
	}

	mainBuffer->GetResource()->SetName(L"Geometry Pass Main Buffer");

	//Depth Buffer
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	desc.Format = depthBufferFormat;
	desc.MipLevels = 1;

	DX12_TextureInitInfo info2;
	info2.resourceDesc = &desc;
	info.initialState = initialDepthBufferState;
	info.clearValue.Format = desc.Format;
	info.clearValue.DepthStencil.Depth = 0.0f;
	info.clearValue.DepthStencil.Stencil = 0;

	depthBuffer = new DX12_DepthBufferTextureInfo(info);
	GADGET_BASIC_ASSERT(depthBuffer != nullptr);
	GADGET_BASIC_ASSERT(depthBuffer->GetResource() != nullptr);
	if(depthBuffer->GetResource() == nullptr){
		GADGET_LOG_ERROR(SID("RENDER"), "Couuld not create DX12_RenderTextureInfo for the geometry depth buffer!");
		return ErrorCode::Constructor_Failed;
	}
	depthBuffer->GetResource()->SetName(L"Geometry Pass Depth Buffer");

	return ErrorCode::OK;
}

ErrorCode DX12_GeometryPass::CreateRootSignatureAndPSO(){
	GADGET_BASIC_ASSERT(rootSignature == nullptr && pipelineStateObject == nullptr);

	//Root Signature
	DX12_Helpers::DX12_RootParameter params[1]{};
	params[0].InitAsConstants(1, D3D12_SHADER_VISIBILITY_PIXEL, 1);
	DX12_Helpers::DX12_RootSignatureDesc desc{ &params[0], _countof(params) };
	desc.Flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	rootSignature.Attach(desc.Create(DX12::GetInstance().MainDevice()));
	GADGET_BASIC_ASSERT(rootSignature != nullptr);
	if(rootSignature == nullptr){
		Debug::Log("Could not create root signature!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}
	rootSignature->SetName(L"Geometry Pass RootSignature");

	//Pipeline State Object
	struct{
		DX12_Helpers::DX12_PipelineStateSubObject_RootSignature			rootSignaturePSO{ rootSignature.Get()};
		DX12_Helpers::DX12_PipelineStateSubObject_VS					vs{ DX12_ShaderHandler::GetEngineShader(EngineShader::ID::TestShader_VS) };
		DX12_Helpers::DX12_PipelineStateSubObject_PS					ps{ DX12_ShaderHandler::GetEngineShader(EngineShader::ID::TestShader_PS) };
		DX12_Helpers::DX12_PipelineStateSubObject_PrimitiveTopology		primitiveTopology{ D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE };
		DX12_Helpers::DX12_PipelineStateSubObject_RenderTargetFormats	renderTargetFormats;
		DX12_Helpers::DX12_PipelineStateSubObject_DepthStencilFormat	depthStencilFormat{ depthBufferFormat };
		DX12_Helpers::DX12_PipelineStateSubObject_Rasterizer			rasterizer{ DX12_Helpers::RasterizerNoCulling };
		DX12_Helpers::DX12_PipelineStateSubObject_DepthStencil1			depth{ DX12_Helpers::DepthStencilDisabled };
	} psoStream;

	D3D12_RT_FORMAT_ARRAY rtfArray{};
	rtfArray.NumRenderTargets = 1;
	rtfArray.RTFormats[0] = mainBufferFormat;

	psoStream.renderTargetFormats = rtfArray;

	pipelineStateObject.Attach(DX12_Helpers::CreatePipelineState(DX12::GetInstance().MainDevice(), &psoStream, sizeof(psoStream)));
	GADGET_BASIC_ASSERT(pipelineStateObject != nullptr);
	if(pipelineStateObject == nullptr){
		Debug::Log("Could not create pipeline state object!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}
	pipelineStateObject->SetName(L"Geometry Pass PipelineStateObject");

	return ErrorCode::OK;
}
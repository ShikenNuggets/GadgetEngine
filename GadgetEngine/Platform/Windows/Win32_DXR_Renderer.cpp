#include "Win32_DXR_Renderer.h"

#include "App.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_ShaderHandler.h"
#include "Graphics/DX12/DXR/DXR.h"
#include "Platform/Windows/Win32_Window.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

struct TestVertex{
	Vector3 position;
	Color color;
};

constexpr DXGI_FORMAT mainBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
constexpr D3D12_RESOURCE_STATES initialMainBufferState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

Win32_DXR_Renderer::Win32_DXR_Renderer(int w_, int h_, int x_, int y_) : Renderer(API::DX12), dx12(nullptr), dxr(nullptr), renderSurfacePtr(nullptr), rootSignature(nullptr), pipelineState(nullptr), vertexBuffer(nullptr), mainBuffer(nullptr){
	window = std::make_unique<Win32_Window>(w_, h_, x_, y_);

	Win32_Window* win32Window = dynamic_cast<Win32_Window*>(window.get());
	GADGET_ASSERT(win32Window != nullptr, "Win32 Renderer requires a Win32 window!");

	DX12_StartupOptions options{};
	options.requireDXR = true;
	options.closeCommandListOnInit = false;

#ifdef GADGET_DEBUG
	options.isDebug = true;
	options.gpuBasedValidation = App::GetInstance().GetConfig().GetOptionBool(EngineVars::Render::gpuValidationKey);
	options.dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif // GADGET_DEBUG

	dx12 = &DX12::GetInstance(options);

	renderSurfacePtr = new DX12_RenderSurface(window.get(), w_, h_);
	auto err = dx12->CreateSwapChainForSurface(renderSurfacePtr);
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create swapchain! Error Code: " + std::to_string((uint32_t)err), __FILE__, __LINE__);
	}

	Color clearColor = Color::Black();
#ifdef GADGET_DEBUG
	clearColor = Color::DarkGray();
#endif //GADGET_DEBUG

	//------------------------------------------------------------------------------------------------------//
	//-----------------------------------Main Buffer--------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------//
	D3D12_RESOURCE_DESC desc{};
	desc.Alignment = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	desc.Format = mainBufferFormat;
	desc.Width = x_;
	desc.Height = y_;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.MipLevels = 0;
	desc.SampleDesc = { 1, 0 };

	//Main Buffer
	DX12_TextureInitInfo info;
	info.resourceDesc = &desc;
	info.initialState = initialMainBufferState;
	info.clearValue.Format = desc.Format;
	info.clearValue.Color[0] = clearColor.r;
	info.clearValue.Color[1] = clearColor.g;
	info.clearValue.Color[2] = clearColor.b;
	info.clearValue.Color[3] = clearColor.a;

	mainBuffer = new DX12_RenderTextureInfo(info);
	GADGET_BASIC_ASSERT(mainBuffer != nullptr);
	GADGET_BASIC_ASSERT(mainBuffer->GetResource() != nullptr);
	if(mainBuffer->GetResource() == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Couuld not create DX12_RenderTextureInfo for the main geometry buffer!", __FILE__, __LINE__);
	}

	mainBuffer->GetResource()->SetName(L"DXR Renderer Main Buffer");
	//------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------//

	(void)DX12_ShaderHandler::Initialize();
	SetupTestAssets();

	dxr = &DXR::GetInstance(window->GetSize(), vertexBuffer.Get());
}

Win32_DXR_Renderer::~Win32_DXR_Renderer(){
	DX12_ShaderHandler::Shutdown();
}

void Win32_DXR_Renderer::PostInit(){}

void Win32_DXR_Renderer::Render(const Scene* scene_){
	(void)dx12->GfxCommand()->BeginFrame();
	auto* cmdList = dx12->GfxCommand()->CommandList();

	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	cmdList->SetPipelineState(pipelineState.Get());
	cmdList->RSSetViewports(1, &renderSurfacePtr->Viewport());
	cmdList->RSSetScissorRects(1, &renderSurfacePtr->ScissorRect());

	auto& resourceBarriers = dx12->ResourceBarriers();
	resourceBarriers.AddTransitionBarrier(mainBuffer->GetResource(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	resourceBarriers.ApplyAllBarriers(cmdList);

	auto cpuStart = dx12->RTVHeap().CPUStart();
	cmdList->OMSetRenderTargets(1, &cpuStart, FALSE, nullptr);

	//The Actual Rendering Part
	std::vector<ID3D12DescriptorHeap*> heaps = { dx12->SRVHeap().Heap() };
	cmdList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());

	resourceBarriers.AddTransitionBarrier(dxr->OutputResource(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	resourceBarriers.ApplyAllBarriers(cmdList);

	D3D12_DISPATCH_RAYS_DESC desc = {};
	uint32_t rayGenerationSectionSizeInBytes = dxr->SBTHelper().GetRayGenSectionSize();
	desc.RayGenerationShaderRecord.StartAddress = dxr->SBTStorage()->GetGPUVirtualAddress();
	desc.RayGenerationShaderRecord.SizeInBytes = rayGenerationSectionSizeInBytes;

	uint32_t missSectionSizeInBytes = dxr->SBTHelper().GetMissSectionSize();
	desc.MissShaderTable.StartAddress = dxr->SBTStorage()->GetGPUVirtualAddress() + rayGenerationSectionSizeInBytes;
	desc.MissShaderTable.SizeInBytes = missSectionSizeInBytes;
	desc.MissShaderTable.StrideInBytes = dxr->SBTHelper().GetMissEntrySize();

	uint32_t hitGroupSectionSize = dxr->SBTHelper().GetHitGroupSectionSize();
	desc.HitGroupTable.StartAddress = dxr->SBTStorage()->GetGPUVirtualAddress() + rayGenerationSectionSizeInBytes + missSectionSizeInBytes;
	desc.HitGroupTable.SizeInBytes = hitGroupSectionSize;
	desc.HitGroupTable.StrideInBytes = dxr->SBTHelper().GetHitGroupEntrySize();

	desc.Width = renderSurfacePtr->GetWidth();
	desc.Height = renderSurfacePtr->GetHeight();
	desc.Depth = 1;

	cmdList->SetPipelineState1(dxr->RTStateObject());
	cmdList->DispatchRays(&desc);

	resourceBarriers.AddTransitionBarrier(dxr->OutputResource(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
	resourceBarriers.ApplyAllBarriers(cmdList);

	resourceBarriers.AddTransitionBarrier(mainBuffer->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_DEST);
	resourceBarriers.ApplyAllBarriers(cmdList);

	cmdList->CopyResource(mainBuffer->GetResource(), dxr->OutputResource());

	resourceBarriers.AddTransitionBarrier(mainBuffer->GetResource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);
	resourceBarriers.ApplyAllBarriers(cmdList);

	(void)dx12->GfxCommand()->EndFrame(renderSurfacePtr);
}

void Win32_DXR_Renderer::ClearScreen(){}

void Win32_DXR_Renderer::SetClearColor(const Color& color_){}

void Win32_DXR_Renderer::SetViewportRect(const Rect& rect_){}

void Win32_DXR_Renderer::OnResize(int newWidth_, int newHeight_){}

void Win32_DXR_Renderer::SetWindingOrder(WindingOrder order_){}

void Win32_DXR_Renderer::SetCullFace(CullFace cullFace_){}

Shader* Win32_DXR_Renderer::GenerateAPIShader(StringID shaderResource_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

MeshInfo* Win32_DXR_Renderer::GenerateAPIMeshInfo(const Mesh& mesh_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

MeshInfo* Win32_DXR_Renderer::GenerateAPIDynamicMeshInfo(size_t numVertices_, size_t numIndices_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

TextureInfo* Win32_DXR_Renderer::GenerateAPITextureInfo(const Texture& texture_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

FontInfo* Win32_DXR_Renderer::GenerateAPIFontInfo(const FreetypeFont& font_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

ErrorCode Win32_DXR_Renderer::SetupTestAssets(){
	GADGET_BASIC_ASSERT(dx12 != nullptr);
	DX12_Helpers::DX12_RootSignatureDesc rootSignatureDesc = DX12_Helpers::DX12_RootSignatureDesc(nullptr, 0, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> signatureBlob;
	ComPtr<ID3DBlob> errorBlob;
	
	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, signatureBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
	if(FAILED(hr) || signatureBlob == nullptr){
		return ErrorCode::D3D12_Error;
	}

	hr = dx12->MainDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
	if(FAILED(hr) || rootSignature == nullptr){
		return ErrorCode::D3D12_Error;
	}

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, static_cast<UINT>(std::size(inputElementDescs)) };
	psoDesc.pRootSignature = rootSignature.Get();
	psoDesc.VS = DX12_ShaderHandler::GetEngineShader(EngineShader::ID::TestShader_VS);
	psoDesc.PS = DX12_ShaderHandler::GetEngineShader(EngineShader::ID::TestShader_PS);
	psoDesc.RasterizerState = DX12_Helpers::RasterizerNoCulling;
	psoDesc.BlendState = DX12_Helpers::BlendDisabled;
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;

	hr = dx12->MainDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
	if(FAILED(hr) || pipelineState == nullptr){
		return ErrorCode::D3D12_Error;
	}

	dx12->GfxCommand()->CommandList()->SetPipelineState(pipelineState.Get());

	TestVertex triangleVertices[]{
		{ Vector3(0.0f, 0.25f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Vector3(0.25f, -0.25f, 0.0f), Color(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(-0.25f, -0.25f, 0.0f), Color(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	const UINT vertexBufferSize = sizeof(triangleVertices);

	vertexBuffer.Attach(DX12_Helpers::CreateBuffer(dx12->MainDevice(), triangleVertices, vertexBufferSize, true, D3D12_RESOURCE_STATE_GENERIC_READ));

	(void)dx12->GfxCommand()->EndFrame(nullptr);

	return ErrorCode::OK;
}
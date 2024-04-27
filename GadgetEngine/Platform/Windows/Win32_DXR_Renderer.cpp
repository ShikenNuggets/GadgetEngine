#include "Win32_DXR_Renderer.h"

#include "App.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_ShaderHandler.h"
#include "Graphics/DX12/DXR/DXR.h"
#include "Platform/Windows/Win32_Window.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

Win32_DXR_Renderer::Win32_DXR_Renderer(int w_, int h_, int x_, int y_) : Renderer(API::DX12), dx12(nullptr), dxr(nullptr), renderSurfacePtr(nullptr), rootSignature(nullptr), pipelineState(nullptr), vertexBuffer(nullptr)/*, mainBuffer(nullptr)*/{
	window = std::make_unique<Win32_Window>(w_, h_, x_, y_, renderAPI);

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

	(void)DX12_ShaderHandler::Initialize();
	SetupTestAssets();

	dxr = &DXR::GetInstance(window->GetSize(), vertexBuffer.Get());
}

Win32_DXR_Renderer::~Win32_DXR_Renderer(){
	DX12_ShaderHandler::Shutdown();
}

void Win32_DXR_Renderer::PostInit(){}

void Win32_DXR_Renderer::Render([[maybe_unused]] const Scene* scene_){
	(void)dx12->GfxCommand()->BeginFrame();

	dx12->ProcessDeferredReleases(dx12->CurrentFrameIndex());

	auto* mainBuffer = renderSurfacePtr->CurrentBackBuffer();
	auto* cmdList = dx12->GfxCommand()->CommandList();

	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	cmdList->SetPipelineState(pipelineState.Get());
	cmdList->RSSetViewports(1, &renderSurfacePtr->Viewport());
	cmdList->RSSetScissorRects(1, &renderSurfacePtr->ScissorRect());

	auto& resourceBarriers = dx12->ResourceBarriers();
	resourceBarriers.AddTransitionBarrier(mainBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	resourceBarriers.ApplyAllBarriers(cmdList);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuStart = D3D12_CPU_DESCRIPTOR_HANDLE(dx12->RTVHeap().CPUStart().ptr + (static_cast<SIZE_T>(renderSurfacePtr->CurrentBackBufferIndex()) * dx12->MainDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
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

	resourceBarriers.AddTransitionBarrier(mainBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_DEST);
	resourceBarriers.ApplyAllBarriers(cmdList);

	cmdList->CopyResource(mainBuffer, dxr->OutputResource());

	resourceBarriers.AddTransitionBarrier(mainBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);
	resourceBarriers.ApplyAllBarriers(cmdList);

	resourceBarriers.AddTransitionBarrier(mainBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	resourceBarriers.ApplyAllBarriers(cmdList);

	(void)dx12->GfxCommand()->EndFrame(renderSurfacePtr);
}

void Win32_DXR_Renderer::ClearScreen(){}

void Win32_DXR_Renderer::SetClearColor([[maybe_unused]] const Color& color_){}

void Win32_DXR_Renderer::SetViewportRect([[maybe_unused]] const Rect& rect_){}

void Win32_DXR_Renderer::OnResize([[maybe_unused]] int newWidth_, [[maybe_unused]] int newHeight_){}

void Win32_DXR_Renderer::SetWindingOrder([[maybe_unused]] WindingOrder order_){}

void Win32_DXR_Renderer::SetCullFace([[maybe_unused]] CullFace cullFace_){}

Shader* Win32_DXR_Renderer::GenerateAPIShader([[maybe_unused]] StringID shaderResource_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

MeshInfo* Win32_DXR_Renderer::GenerateAPIMeshInfo([[maybe_unused]] const Mesh& mesh_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

MeshInfo* Win32_DXR_Renderer::GenerateAPIDynamicMeshInfo([[maybe_unused]] size_t numVertices_, [[maybe_unused]] size_t numIndices_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

TextureInfo* Win32_DXR_Renderer::GenerateAPITextureInfo([[maybe_unused]] const Texture& texture_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

FontInfo* Win32_DXR_Renderer::GenerateAPIFontInfo([[maybe_unused]] const FreetypeFont& font_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

struct TestVertex{
	Vector3 position;
	Color color;
};

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

	(void)dx12->GfxCommand()->ExecuteCommandsImmediate();

	return ErrorCode::OK;
}
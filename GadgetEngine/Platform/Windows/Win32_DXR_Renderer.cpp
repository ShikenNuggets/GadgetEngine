#include "Win32_DXR_Renderer.h"

#include "App.h"
#include "Graphics/Components/CameraComponent.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_ShaderHandler.h"
#include "Graphics/DX12/DX12_UploadHandler.h"
#include "Graphics/DX12/DXR/DXR.h"
#include "Graphics/DX12/DXR/DXR_MeshInfo.h"
#include "Platform/Windows/Win32_Window.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

Win32_DXR_Renderer::Win32_DXR_Renderer(int w_, int h_, int x_, int y_) : Renderer(API::DX12), dx12(nullptr), dxr(nullptr), renderSurfacePtr(nullptr), rootSignature(nullptr), pipelineState(nullptr), vertexBuffer(nullptr){
	window = std::make_unique<Win32_Window>(w_, h_, x_, y_, renderAPI);

	Win32_Window* win32Window = dynamic_cast<Win32_Window*>(window.get());
	GADGET_ASSERT(win32Window != nullptr, "Win32 Renderer requires a Win32 window!");

	DX12_StartupOptions options{};
	options.requireDXR = true;
	options.closeCommandListOnInit = false;

#ifdef GADGET_DEBUG
	options.breakOnErrors = true;
	options.gpuBasedValidation = App::GetInstance().GetConfig().GetOptionBool(EngineVars::Render::gpuValidationKey);
	options.dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif //GADGET_DEBUG

#if defined(GADGET_DEBUG) && not defined(GADGET_DEVELOP)
	options.breakOnWarnings = true;
#endif //GADGET_DEBUG && !GADGET_RELEASE

	dx12 = &DX12::GetInstance(options);

	renderSurfacePtr = new DX12_RenderSurface(window.get(), w_, h_);
	auto err = dx12->CreateSwapChainForSurface(renderSurfacePtr);
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create swapchain!", err, __FILE__, __LINE__);
	}

	err = dx12->FlushAndSetFrameIndex(renderSurfacePtr->CurrentBackBufferIndex()); //This SHOULD always be zero on startup... but just in case it's not
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Something went wrong when setting the new frame index!", err, __FILE__, __LINE__);
	}

	Color clearColor = Color::Black();
#ifdef GADGET_DEBUG
	clearColor = Color::DarkGray();
#endif //GADGET_DEBUG

	err = DX12_ShaderHandler::Initialize();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Could not initialize shader handler!", err, __FILE__, __LINE__);
	}

	DX12_UploadHandler::GetInstance(dx12->MainDevice());

	err = SetupTestAssets();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Could not set up test assets!", err, __FILE__, __LINE__);
	}

	//TODO - Test stuff. This leaks memory
	DXR_MeshInfo* triangle1Mesh = new DXR_MeshInfo(3, vertexBuffer.Get(), indexBuffer.Get());
	DXR_MeshInfo* planeMesh = new DXR_MeshInfo(6, planeVertexBuffer.Get(), planeIndexBuffer.Get());

	std::vector<DXR_MeshInfo*> meshes;
	meshes.push_back(triangle1Mesh);
	meshes.push_back(planeMesh);
	meshes.shrink_to_fit();

	dxr = &DXR::GetInstance(window->GetSize(), meshes);
}

Win32_DXR_Renderer::~Win32_DXR_Renderer(){
	DX12_UploadHandler::DeleteInstance();
	DX12_ShaderHandler::Shutdown();

	//TODO - The ownership semantics for the renderSurfacePtr are kinda weird
	if(renderSurfacePtr != nullptr){
		delete renderSurfacePtr;
		renderSurfacePtr = nullptr;
		window->SetRenderSurface(nullptr);
	}

	(void)DX12::DeleteInstance(); //Not a whole lot we can do for error handling in a destructor
}

void Win32_DXR_Renderer::PostInit(){}

void Win32_DXR_Renderer::Render([[maybe_unused]] const Scene* scene_){
	GADGET_BASIC_ASSERT(scene_ != nullptr);
	GADGET_BASIC_ASSERT(dx12 != nullptr && dx12->IsInitialized());
	GADGET_BASIC_ASSERT(renderSurfacePtr != nullptr);

	if(dx12 == nullptr || !dx12->IsInitialized() || dx12->GfxCommand() == nullptr){
		GADGET_LOG_ERROR(SID("RENDER"), "DX12 was not correctly initialized, cannot render!");
		return;
	}

	if(renderSurfacePtr == nullptr){
		GADGET_LOG_ERROR(SID("RENDER"), "DX12 render surface was not correctly initialized, cannot render!");
		return;
	}

	auto cams = scene_->GetAllComponentsInScene<CameraComponent>();
	if(cams.size() <= 0){
		return;
	}

	dxr->UpdateCameraBuffer(cams[0]->GetUpdatedViewMatrix(), cams[0]->GetUpdatedProjectionMatrix());

	auto err = dx12->GfxCommand()->BeginFrame();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Could not setup DX12_Command for the next frame!", err, __FILE__, __LINE__);
	}

	dx12->ProcessDeferredReleasesForCurrentFrame();

	ID3D12Resource* backBuffer = renderSurfacePtr->CurrentBackBuffer();
	GADGET_BASIC_ASSERT(backBuffer != nullptr);
	if(backBuffer == nullptr){
		GADGET_LOG_ERROR(SID("RENDER"), "Back buffer for current frame was null, cannot render!");
		return;
	}

	ID3D12_GraphicsCommandList* cmdList = dx12->GfxCommand()->CommandList();
	GADGET_BASIC_ASSERT(cmdList != nullptr);
	if(cmdList == nullptr){
		GADGET_LOG_ERROR(SID("RENDER"), "DX12 was not correctly initialized, cannot render!");
		return;
	}

	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//cmdList->SetPipelineState(pipelineState.Get());
	cmdList->RSSetViewports(1, &renderSurfacePtr->Viewport());
	cmdList->RSSetScissorRects(1, &renderSurfacePtr->ScissorRect());

	auto& resourceBarriers = dx12->ResourceBarriers();
	resourceBarriers.AddTransitionBarrier(backBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	resourceBarriers.ApplyAllBarriers(cmdList);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuStart = D3D12_CPU_DESCRIPTOR_HANDLE(dx12->RTVHeap().CPUStart().ptr + (static_cast<SIZE_T>(renderSurfacePtr->CurrentBackBufferIndex()) * dx12->RTVHeap().DescriptorSize()));
	cmdList->OMSetRenderTargets(1, &cpuStart, FALSE, nullptr);

	//The Actual Rendering Part
	dxr->UpdateTopLevelAS();

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

	resourceBarriers.AddTransitionBarrier(backBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_DEST);
	resourceBarriers.ApplyAllBarriers(cmdList);

	cmdList->CopyResource(backBuffer, dxr->OutputResource());

	//TODO - We can transition directly to the present state. Is there any reason why we shouldn't?
	resourceBarriers.AddTransitionBarrier(backBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);
	resourceBarriers.ApplyAllBarriers(cmdList);

	resourceBarriers.AddTransitionBarrier(backBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	resourceBarriers.ApplyAllBarriers(cmdList);

	err = dx12->GfxCommand()->EndFrame(renderSurfacePtr);
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Could not end command setup for the current frame!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
}

void Win32_DXR_Renderer::ClearScreen(){}

void Win32_DXR_Renderer::SetClearColor([[maybe_unused]] const Color& color_){}

void Win32_DXR_Renderer::SetViewportRect([[maybe_unused]] const Rect& rect_){}

void Win32_DXR_Renderer::OnResize([[maybe_unused]] int newWidth_, [[maybe_unused]] int newHeight_){}

void Win32_DXR_Renderer::SetWindingOrder([[maybe_unused]] WindingOrder order_){}

void Win32_DXR_Renderer::SetCullFace([[maybe_unused]] CullFace cullFace_){}

Shader* Win32_DXR_Renderer::GenerateAPIShader([[maybe_unused]] StringID shaderResource_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

MeshInfo* Win32_DXR_Renderer::GenerateAPIMeshInfo(const Mesh& mesh_){ return new DXR_MeshInfo(mesh_); }

MeshInfo* Win32_DXR_Renderer::GenerateAPIDynamicMeshInfo([[maybe_unused]] size_t numVertices_, [[maybe_unused]] size_t numIndices_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

TextureInfo* Win32_DXR_Renderer::GenerateAPITextureInfo([[maybe_unused]] const Texture& texture_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

FontInfo* Win32_DXR_Renderer::GenerateAPIFontInfo([[maybe_unused]] const FreetypeFont& font_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

struct TestVertex{
	Gadget::Vector3 position;
	Gadget::Color color;
};

ErrorCode Win32_DXR_Renderer::SetupTestAssets(){
	//------------------------------------------------------------//
	//------------------ Root Signature --------------------------//
	//------------------------------------------------------------//
	GADGET_BASIC_ASSERT(dx12 != nullptr && dx12->IsInitialized());

	DX12_Helpers::DX12_RootParameter constantParameter{};
	DX12_Helpers::DX12_DescriptorRange range = DX12_Helpers::DX12_DescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	constantParameter.InitAsTable(D3D12_SHADER_VISIBILITY_ALL, &range, 1);

	DX12_Helpers::DX12_RootSignatureDesc rootSignatureDesc = DX12_Helpers::DX12_RootSignatureDesc(&constantParameter, 1, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	rootSignature.Attach(rootSignatureDesc.Create(dx12->MainDevice()));

	//------------------------------------------------------------//
	//------------------ Pipeline State --------------------------//
	//------------------------------------------------------------//
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, static_cast<UINT>(std::size(inputElementDescs)) };
	psoDesc.pRootSignature = rootSignature.Get();
	psoDesc.VS = DX12_ShaderHandler::GetEngineShader(EngineShader::ID::TestShader_VS);
	psoDesc.PS = DX12_ShaderHandler::GetEngineShader(EngineShader::ID::TestShader_PS);
	psoDesc.RasterizerState = DX12_Helpers::RasterizerBackFaceCulling;
	psoDesc.BlendState = DX12_Helpers::BlendDisabled;
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;

	HRESULT hr = dx12->MainDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
	if(FAILED(hr) || pipelineState == nullptr){
		return ErrorCode::D3D12_Error;
	}

	dx12->GfxCommand()->CommandList()->SetPipelineState(pipelineState.Get());

	//------------------------------------------------------------//
	//------------------ Triangles -------------------------------//
	//------------------------------------------------------------//
	float aspect = 16.0f / 9.0f;

	TestVertex triangleVertices[]{
		{ Vector3(0.0f, 0.25f * aspect, 0.0f), Color::Red() },
		{ Vector3(0.25f, -0.25f * aspect, 0.0f), Color::Green() },
		{ Vector3(-0.25f, -0.25f * aspect, 0.0f), Color::Blue() }
	};
	std::vector<UINT> indices = { 0, 1, 2 };

	constexpr UINT vertexBufferSize = sizeof(triangleVertices);
	const UINT indexBufferSize = static_cast<UINT>(indices.size()) * sizeof(UINT);

	vertexBuffer.Attach(DX12_Helpers::CreateBuffer(dx12->MainDevice(), triangleVertices, vertexBufferSize, true, D3D12_RESOURCE_STATE_GENERIC_READ));
	indexBuffer.Attach(DX12_Helpers::CreateBuffer(dx12->MainDevice(), indices.data(), indexBufferSize, true, D3D12_RESOURCE_STATE_GENERIC_READ));

	//------------------------------------------------------------//
	//------------------ Plane -----------------------------------//
	//------------------------------------------------------------//
	TestVertex planeVertices[] = {
		{ Vector3(-1.5f, -0.8f,  1.5f), Color::White() }, // 0
		{ Vector3(-1.5f, -0.8f, -1.5f), Color::White() }, // 1
		{ Vector3( 1.5f, -0.8f,  1.5f), Color::White() }, // 2
		//{ Vector3( 1.5f, -0.8f,  1.5f), Color::White() }, // 2
		//{ Vector3(-1.5f, -0.8f, -1.5f), Color::White() }, // 1
		{ Vector3( 1.5f, -0.8f, -1.5f), Color::White() }  // 4
	};
	std::vector<UINT> planeIndices = { 0, 1, 2, 2, 1, 3 };

	constexpr UINT planeBufferSize = sizeof(planeVertices);
	const UINT planeIndexBufferSize = static_cast<UINT>(planeIndices.size()) * sizeof(UINT);

	planeVertexBuffer.Attach(DX12_Helpers::CreateBuffer(dx12->MainDevice(), planeVertices, planeBufferSize, true, D3D12_RESOURCE_STATE_GENERIC_READ));
	planeIndexBuffer.Attach(DX12_Helpers::CreateBuffer(dx12->MainDevice(), planeIndices.data(), planeIndexBufferSize, true, D3D12_RESOURCE_STATE_GENERIC_READ));

	//------------------------------------------------------------//
	//------------------ Execute Commands ------------------------//
	//------------------------------------------------------------//
	auto err = dx12->GfxCommand()->ExecuteCommandsImmediate();
	if(err != ErrorCode::OK){
		return err;
	}

	return ErrorCode::OK;
}
#include "Win32_DX12_Renderer.h"

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

#include "App.h"
#include "Win32_Window.h"
#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_DescriptorHeap.h"
#include "Graphics/DX12/DX12_Helpers.h"
#include "Graphics/DX12/DX12_RenderSurface.h"
#include "Graphics/DX12/DX12_ShaderHandler.h"
#include "Graphics/DX12/DX12_GeometryPass.h"
#include "Graphics/DX12/DX12_PostProcess.h"

using namespace Gadget;

Win32_DX12_Renderer::Win32_DX12_Renderer(int w_, int h_, int x_, int y_) : Renderer(API::DX12){
	window = std::make_unique<Win32_Window>(w_, h_, x_, y_);

	Win32_Window* win32Window = dynamic_cast<Win32_Window*>(window.get());
	GADGET_ASSERT(win32Window != nullptr, "Win32 Renderer requires a Win32 window!");

	DX12_StartupOptions options;
	
#ifdef GADGET_DEBUG
	options.isDebug = true;
	options.gpuBasedValidation = App::GetInstance().GetConfig().GetOptionBool(EngineVars::Render::gpuValidationKey);
	options.dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif // GADGET_DEBUG

	auto& dx12 = DX12::GetInstance(options);

	renderSurfacePtr = new DX12_RenderSurface(window.get(), w_, h_);
	(void)dx12.CreateSwapChainForSurface(renderSurfacePtr);
	window->SetRenderSurface(renderSurfacePtr);

	auto err = DX12_ShaderHandler::Initialize();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to initialize shader handler!", __FILE__, __LINE__);
	}

	Color clearColor = Color::Black();
	#ifdef GADGET_DEBUG
	clearColor = Color::DarkGray();
	#endif //GADGET_DEBUG

	err = DX12_GeometryPass::Initialize(window->GetSize(), clearColor);
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to initialize geometry pass!", __FILE__, __LINE__);
	}

	err = DX12_PostProcess::Initialize();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to initialize post-processing submodule!", __FILE__, __LINE__);
	}

	GADGET_BASIC_ASSERT(dx12.IsInitialized());
}

Win32_DX12_Renderer::~Win32_DX12_Renderer(){
	auto& dx12 = DX12::GetInstance();

	auto err = dx12.PreShutdown();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "An error occurred in the pre-shutdown stage!", __FILE__, __LINE__);
	}

	DX12_PostProcess::Shutdown();
	DX12_GeometryPass::Shutdown();
	DX12_ShaderHandler::Shutdown();

	if(renderSurfacePtr != nullptr){
		delete renderSurfacePtr;
		renderSurfacePtr = nullptr;
		window->SetRenderSurface(nullptr);
	}

	(void)DX12::DeleteInstance();
	window.reset();
}

void Win32_DX12_Renderer::PostInit(){
	Renderer::PostInit();
}

void Win32_DX12_Renderer::Render(const Scene* scene_){
	auto& dx12 = DX12::GetInstance();

	GADGET_BASIC_ASSERT(dx12.IsInitialized());
	GADGET_BASIC_ASSERT(dx12.GfxCommand() != nullptr);
	GADGET_BASIC_ASSERT(scene_ != nullptr);

	auto gfxCommand = dx12.GfxCommand();
	if(gfxCommand == nullptr){
		Debug::Log(SID("RENDER"), "Tried to render with no command list, is DX12 initialized?", Debug::Warning, __FILE__, __LINE__);
		return;
	}

	if(renderSurfacePtr == nullptr){
		Debug::Log(SID("RENDER"), "Tried to render with no RenderSurface!", Debug::Warning, __FILE__, __LINE__);
		return;
	}

	(void)gfxCommand->BeginFrame();
	ID3D12_GraphicsCommandList* const cmdList = gfxCommand->CommandList();
	GADGET_BASIC_ASSERT(cmdList != nullptr);

	dx12.ProcessDeferredReleases(dx12.CurrentFrameIndex());

	ID3D12Resource* const currentBackBuffer = renderSurfacePtr->CurrentBackBuffer();

	DX12_GeometryPass::OnResize(window->GetSize()); //TODO - This is overkill

	//Do stuff
	ID3D12DescriptorHeap* const heaps[]{ dx12.SRVHeap().Heap() };
	cmdList->SetDescriptorHeaps(1, &heaps[0]);

	cmdList->RSSetViewports(1, &renderSurfacePtr->Viewport());
	cmdList->RSSetScissorRects(1, &renderSurfacePtr->ScissorRect());

	//TODO - Splitting barriers (instead of one call, do a BEGIN_ONLY and END_ONLY call separately) could theoretically improve performance
	//As of right now it doesn't seem to make any difference, but once we have a real workload we should check
	//Could also make it configurable so engine users can profile and see what makes sense for their game?

	//----------Depth Pre-Pass----------
	auto& resourceBarriers = dx12.ResourceBarriers();

	DX12_GeometryPass::AddTransitionsForDepthPrepass(resourceBarriers);
	resourceBarriers.ApplyAllBarriers(cmdList);

	DX12_GeometryPass::SetRenderTargetsForDepthPrepass(cmdList);
	DX12_GeometryPass::DepthPrepass(cmdList, window->GetSize());

	//----------Geometry Pass-----------
	DX12_GeometryPass::AddTransitionsForGeometryPrepass(resourceBarriers);
	resourceBarriers.ApplyAllBarriers(cmdList);

	DX12_GeometryPass::SetRenderTargetsForGeometryPass(cmdList);
	DX12_GeometryPass::Render(cmdList, window->GetSize());
	
	DX12_Helpers::TransitionResource(cmdList, currentBackBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//----------Post-Processing-----------
	DX12_GeometryPass::AddTransitionsForPostProcess(resourceBarriers);
	resourceBarriers.ApplyAllBarriers(cmdList);
	DX12_PostProcess::PostProcess(cmdList, DX12_GeometryPass::MainBuffer(), renderSurfacePtr->CurrentRenderTargetView());
	//Afer post-processing

	//----------Finalize Render-----------
	DX12_Helpers::TransitionResource(cmdList, currentBackBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	//Done doing stuff
	(void)gfxCommand->EndFrame(renderSurfacePtr);

	//Do this only at the end
	//TODO - Not actually sure if DX12 needs this, or something else
	window->SwapBuffers();
}

void Win32_DX12_Renderer::ClearScreen(){
	GADGET_ASSERT_NOT_IMPLEMENTED;
}

void Win32_DX12_Renderer::SetClearColor([[maybe_unused]] const Color& color_){
	DX12_GeometryPass::SetClearColor(color_);
}

void Win32_DX12_Renderer::SetViewportRect([[maybe_unused]] const Rect& rect_){
	GADGET_ASSERT(rect_.x >= 0.0f && rect_.x <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.y >= 0.0f && rect_.y <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.w >= 0.0f && rect_.w <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.h >= 0.0f && rect_.h <= 1.0f, "Tried to set invalid viewport rect!");

	GADGET_ASSERT_NOT_IMPLEMENTED;
}

void Win32_DX12_Renderer::OnResize(int width_, int height_){
	if(!postInitComplete){
		return; //New size will be handled correctly when we finish initializing
	}

	(void)DX12::GetInstance().ResizeSurface(renderSurfacePtr, width_, height_);
}

void Win32_DX12_Renderer::SetWindingOrder([[maybe_unused]] WindingOrder order_){
	Renderer::SetWindingOrder(order_);
	GADGET_ASSERT_NOT_IMPLEMENTED;
}

void Win32_DX12_Renderer::SetCullFace([[maybe_unused]] CullFace cullFace_){
	Renderer::SetCullFace(cullFace_);
}

Shader* Win32_DX12_Renderer::GenerateAPIShader([[maybe_unused]] StringID shaderResource_){
	GADGET_ASSERT_NOT_IMPLEMENTED;
	return nullptr;
	//return App::GetResourceManager().LoadResource<GL_Shader>(shaderResource_); //TODO - This feels bad...
}

MeshInfo* Win32_DX12_Renderer::GenerateAPIMeshInfo([[maybe_unused]] const Mesh& mesh_){
	GADGET_ASSERT_NOT_IMPLEMENTED;
	return nullptr;
	//return new GL_MeshInfo(mesh_);
}

MeshInfo* Win32_DX12_Renderer::GenerateAPIDynamicMeshInfo([[maybe_unused]] size_t numVertices_, [[maybe_unused]] size_t numIndices_){
	GADGET_ASSERT_NOT_IMPLEMENTED;
	return nullptr;
	//return new GL_DynamicMeshInfo(numVertices_, numIndices_);
}

TextureInfo* Win32_DX12_Renderer::GenerateAPITextureInfo([[maybe_unused]] const Texture& texture_){
	GADGET_ASSERT_NOT_IMPLEMENTED;
	return nullptr;
	//return new GL_TextureInfo(texture_);
}

FontInfo* Win32_DX12_Renderer::GenerateAPIFontInfo([[maybe_unused]] const FreetypeFont& font_){
	GADGET_ASSERT_NOT_IMPLEMENTED;
	return nullptr;
	//return new GL_FontInfo(font_);
}
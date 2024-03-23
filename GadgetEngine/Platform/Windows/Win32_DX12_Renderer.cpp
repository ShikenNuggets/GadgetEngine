#include "Win32_DX12_Renderer.h"

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

#include <SDL_syswm.h>

#include "Win32_Window.h"
#include "Graphics/DX12/DX12_Command.h"

using namespace Gadget;

constexpr D3D_FEATURE_LEVEL minimumFeatureLevel = D3D_FEATURE_LEVEL_11_0; //TODO - Make this configurable

Win32_DX12_Renderer::Win32_DX12_Renderer(int w_, int h_, int x_, int y_) : Renderer(API::DX12), mainDevice(nullptr), dxgiFactory(nullptr){
	window = std::make_unique<Win32_Window>(w_, h_, x_, y_);

	Win32_Window* win32Window = dynamic_cast<Win32_Window*>(window.get());
	GADGET_ASSERT(win32Window != nullptr, "Win32 Renderer requires a Win32 window!");

	//SDL_SysWMinfo wmInfo{};
	//SDL_VERSION(&wmInfo.version);
	//SDL_GetWindowWMInfo(win32Window->GetSDLWindow(), &wmInfo);
	//HWND hwnd = wmInfo.info.win.window;

	HRESULT result;
	uint32_t dxgiFactoryFlags = 0;
	
#ifdef GADGET_DEBUG
	{
		Microsoft::WRL::ComPtr<ID3D12Debug3> debugInterface;
		result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface));
		if(FAILED(result)){
			Debug::ThrowFatalError(SID("RENDER"), "Failed to get D3D12 Debug Interface!", __FILE__, __LINE__);
		}

		debugInterface->EnableDebugLayer();
		//debugInterface->SetEnableGPUBasedValidation(TRUE); //TODO - Make this configurable since it significantly affects performance

		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif // GADGET_DEBUG
	
	result = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
	if(FAILED(result)){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to create DXGI Factory!", __FILE__, __LINE__);
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter4> mainAdapter;
	mainAdapter.Attach(DetermineMainAdapter());
	if(!mainAdapter){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to determine main DXGI adapter! You may need to update your graphics card drivers", __FILE__, __LINE__);
	}

	D3D_FEATURE_LEVEL maxFeatureLevel = GetMaxFeatureLevel(mainAdapter.Get());
	GADGET_BASIC_ASSERT(maxFeatureLevel >= minimumFeatureLevel);
	if(maxFeatureLevel < minimumFeatureLevel){
		Debug::ThrowFatalError(SID("RENDER"), "Max supported feature level is too low, something is wrong with Win32_DX12_Renderer::DetermineMainAdapter. Also, you may need to update your graphics card drivers", __FILE__, __LINE__);
	}

	result = D3D12CreateDevice(mainAdapter.Get(), maxFeatureLevel, IID_PPV_ARGS(&mainDevice));
	if(FAILED(result) || !mainDevice){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to create D3D12Device!", __FILE__, __LINE__);
	}

	mainDevice->SetName(L"MAIN DEVICE");
	Debug::Log(SID("RENDER"), "Created new D3D12 device with name [MAIN DEVICE]", Debug::Info, __FILE__, __LINE__);

#ifdef GADGET_DEBUG
	{
		Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
		result = mainDevice->QueryInterface(IID_PPV_ARGS(&infoQueue));
		if(FAILED(result)){
			Debug::ThrowFatalError(SID("RENDER"), "Failed to query D3D12Device!", __FILE__, __LINE__);
		}

		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	}
#endif // GADGET_DEBUG
}

Win32_DX12_Renderer::~Win32_DX12_Renderer(){
	if(dxgiFactory){
		dxgiFactory->Release();
		dxgiFactory = nullptr;
	}

	#ifdef GADGET_DEBUG
	{
		Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
		HRESULT result = mainDevice->QueryInterface(IID_PPV_ARGS(&infoQueue));
		if(FAILED(result) || !infoQueue){
			Debug::ThrowFatalError(SID("RENDER"), "Failed to query D3D12Device info queue!", __FILE__, __LINE__);
		}

		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, false);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, false);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, false);
		infoQueue->Release();

		Microsoft::WRL::ComPtr<ID3D12DebugDevice2> debugDevice;
		result = mainDevice->QueryInterface(IID_PPV_ARGS(&debugDevice));
		if(FAILED(result)){
			Debug::ThrowFatalError(SID("RENDER"), "Failed to query D3D12 debug device!", __FILE__, __LINE__);
		}

		if(mainDevice){
			mainDevice->Release();
			mainDevice = nullptr;
		}

		//Check for leaks
		result = debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		if(FAILED(result)){
			Debug::ThrowFatalError(SID("RENDER"), "Failed to report live objects from D3D12 debug device!", __FILE__, __LINE__);
		}
	}
	#endif // GADGET_DEBUG

	if(mainDevice){
		mainDevice->Release();
		mainDevice = nullptr;
	}

	window.reset();
}

void Win32_DX12_Renderer::PostInit(){
	Renderer::PostInit();
}

void Win32_DX12_Renderer::Render(const Scene* scene_){
	GADGET_ASSERT_NOT_IMPLEMENTED;

	//Do this only at the end
	window.get()->SwapBuffers();
}

void Win32_DX12_Renderer::ClearScreen(){
	GADGET_ASSERT_NOT_IMPLEMENTED;
}

void Win32_DX12_Renderer::SetClearColor(const Color& color_){
	GADGET_ASSERT_NOT_IMPLEMENTED;
}

void Win32_DX12_Renderer::SetViewportRect(const Rect& rect_){
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
}

void Win32_DX12_Renderer::SetWindingOrder(WindingOrder order_){
	Renderer::SetWindingOrder(order_);
	GADGET_ASSERT_NOT_IMPLEMENTED;
}

void Win32_DX12_Renderer::SetCullFace(CullFace cullFace_){
	Renderer::SetCullFace(cullFace_);
}

Shader* Win32_DX12_Renderer::GenerateAPIShader(StringID shaderResource_){
	GADGET_ASSERT_NOT_IMPLEMENTED;
	return nullptr;
	//return App::GetResourceManager().LoadResource<GL_Shader>(shaderResource_); //TODO - This feels bad...
}

MeshInfo* Win32_DX12_Renderer::GenerateAPIMeshInfo(const Mesh& mesh_){
	GADGET_ASSERT_NOT_IMPLEMENTED;
	return nullptr;
	//return new GL_MeshInfo(mesh_);
}

MeshInfo* Win32_DX12_Renderer::GenerateAPIDynamicMeshInfo(size_t numVertices_, size_t numIndices_){
	GADGET_ASSERT_NOT_IMPLEMENTED;
	return nullptr;
	//return new GL_DynamicMeshInfo(numVertices_, numIndices_);
}

TextureInfo* Win32_DX12_Renderer::GenerateAPITextureInfo(const Texture& texture_){
	GADGET_ASSERT_NOT_IMPLEMENTED;
	return nullptr;
	//return new GL_TextureInfo(texture_);
}

FontInfo* Win32_DX12_Renderer::GenerateAPIFontInfo(const FreetypeFont& font_){
	GADGET_ASSERT_NOT_IMPLEMENTED;
	return nullptr;
	//return new GL_FontInfo(font_);
}

//------------------------------------------------------------------------------------------------------------

IDXGIAdapter4* Win32_DX12_Renderer::DetermineMainAdapter(){
	IDXGIAdapter4* adapter = nullptr;

	for(int i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND; i++){
		if(SUCCEEDED(D3D12CreateDevice(adapter, minimumFeatureLevel, __uuidof(ID3D12Device), nullptr))){
			return adapter;
		}

		if(adapter != nullptr){
			adapter->Release();
		}
	}

	Debug::Log(SID("RENDER"), "Could not find a device adapter that supports the minimum DirectX feature level!", Debug::Warning, __FILE__, __LINE__);
	return nullptr;
}

D3D_FEATURE_LEVEL Win32_DX12_Renderer::GetMaxFeatureLevel(IDXGIAdapter4* adapter_){
	GADGET_BASIC_ASSERT(adapter_ != nullptr);

	//TODO - Should this be configurable?
	constexpr D3D_FEATURE_LEVEL featureLevels[4]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_12_1,
	};

	D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelInfo{};
	featureLevelInfo.NumFeatureLevels = _countof(featureLevels);
	featureLevelInfo.pFeatureLevelsRequested = featureLevels;

	Microsoft::WRL::ComPtr<ID3D12Device> device;
	HRESULT result = D3D12CreateDevice(adapter_, minimumFeatureLevel, IID_PPV_ARGS(&device));
	if(FAILED(result) || !device){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to create D3D12 device at requested minimum feature level!", __FILE__, __LINE__);
	}

	result = device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelInfo, sizeof(featureLevelInfo));
	if(FAILED(result)){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to check feature level support on D3D12 device!", __FILE__, __LINE__);
	}

	return featureLevelInfo.MaxSupportedFeatureLevel;
}
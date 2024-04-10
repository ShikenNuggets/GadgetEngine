#include "DX12.h"

#include <wrl.h>

#include "DX12_Command.h"
#include "DX12_DescriptorHeap.h"
#include "DX12_RenderSurface.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

constexpr D3D_FEATURE_LEVEL minimumFeatureLevel = D3D_FEATURE_LEVEL_11_0; //TODO - Make this configurable

std::unique_ptr<DX12> DX12::instance = nullptr;

DX12::DX12(const DX12_StartupOptions& options_) :	debugLayerEnabled(options_.isDebug), dxgiFactory(nullptr), mainDevice(nullptr), gfxCommand(nullptr), resourceBarriers(),
				rtvDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), dsvDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV),
				srvDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), uavDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),
				deferredReleases(), deferredReleaseFlag(), deferredReleaseMutex(){

	auto err = EnableDebugLayer(options_.gpuBasedValidation);
	if(err != ErrorCode::OK){
		Debug::Log(SID("RENDER"), "[DX12] Could not enable debug layer! Error Code: " + std::to_string((uint32_t)err), Debug::Error, __FILE__, __LINE__);
	}

	err = CreateDevice(options_.dxgiFactoryFlags);
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "[DX12] Could not create device! Error Code: " + std::to_string((uint32_t)err), __FILE__, __LINE__);
	}

	err = BreakOnWarningsAndErrors(options_.isDebug);
	if(err != ErrorCode::OK){
		Debug::Log(SID("RENDER"), "DX12 code will not break on warnings or errors! Error Code: " + std::to_string((uint32_t)err), Debug::Error, __FILE__, __LINE__);
	}

	err = CreateCommandList();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "[DX12] Could not create command list! Error Code: " + std::to_string((uint32_t)err), __FILE__, __LINE__);
	}
	
	err = InitializeDescriptorHeaps();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "[DX12] Could not initialize descriptor heaps! Error Code: " + std::to_string((uint32_t)err), __FILE__, __LINE__);
	}
}

DX12::~DX12(){
	//Destructor should not be called directly
	GADGET_BASIC_ASSERT(dxgiFactory == nullptr);
	GADGET_BASIC_ASSERT(mainDevice == nullptr);
	GADGET_BASIC_ASSERT(gfxCommand == nullptr);

	//Play nice and call shutdown functions anyway
	if(dxgiFactory != nullptr || mainDevice != nullptr || gfxCommand != nullptr){
		(void)PreShutdown();
		(void)Shutdown();
	}
}

DX12& DX12::GetInstance(const DX12_StartupOptions& options_){
	if(instance == nullptr){
		instance = std::make_unique<DX12>(options_);
	}

	GADGET_ASSERT(instance != nullptr, "App instance was somehow nullptr! Nothing will work!");
	return *instance;
}

ErrorCode DX12::DeleteInstance(){
	if(instance != nullptr){
		auto err = instance->PreShutdown();
		if(err != ErrorCode::OK){
			Debug::Log(SID("RENDER"), "DX12::PreShutdown failed!", Debug::Error, __FILE__, __LINE__);
			return err;
		}

		err = instance->Shutdown();
		if(err != ErrorCode::OK){
			Debug::Log(SID("RENDER"), "DX12::Shutdown failed!", Debug::Error, __FILE__, __LINE__);
			return err;
		}

		instance.reset();
	}

	return ErrorCode::OK;
}

ErrorCode DX12::EnableDebugLayer(bool gpuValidation_){
#ifdef GADGET_DEBUG

	Microsoft::WRL::ComPtr<ID3D12_Debug> debugInterface;
	if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugInterface.ReleaseAndGetAddressOf())))){
		debugInterface->EnableDebugLayer();
		debugInterface->SetEnableGPUBasedValidation(gpuValidation_);
	}else{
		Debug::Log(SID("RENDER"), "Failed to get D3D12 Debug Interface!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	debugLayerEnabled = true;

#endif //GADGET_DEBUG

	return ErrorCode::OK;
}

ErrorCode DX12::PreShutdown(){
	ProcessAllDeferredReleases();
	return ErrorCode::OK;
}

ErrorCode DX12::Shutdown(){
	rtvDescriptorHeap.ProcessDeferredFree(0);
	dsvDescriptorHeap.ProcessDeferredFree(0);
	srvDescriptorHeap.ProcessDeferredFree(0);
	uavDescriptorHeap.ProcessDeferredFree(0);

	rtvDescriptorHeap.Release();
	dsvDescriptorHeap.Release();
	srvDescriptorHeap.Release();
	uavDescriptorHeap.Release();

	if(gfxCommand){
		delete gfxCommand;
		gfxCommand = nullptr;
	}

	if(dxgiFactory){
		dxgiFactory.Reset();
	}

	ProcessAllDeferredReleases(); //Some things can't be released until their depending resources are deleted, so we call this one more time at the end

	#ifdef GADGET_DEBUG
	auto err = DebugShutdown();
	if(err != ErrorCode::OK){
		Debug::Log(SID("RENDER"), "An error occurred in DX12::DebugShutdown! Error Code: " + std::to_string((uint32_t)err), Debug::Error, __FILE__, __LINE__);
		return err;
	}

	#endif // GADGET_DEBUG

	if(mainDevice){
		mainDevice->Release();
		mainDevice = nullptr;
	}

	return ErrorCode::OK;
}

ErrorCode DX12::CreateDevice(uint32_t dxgiFactoryFlags_){
	if(FAILED(CreateDXGIFactory2(dxgiFactoryFlags_, IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf())))){
		Debug::Log(SID("RENDER"), "Failed to create DXGI Factory!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter4> mainAdapter;
	mainAdapter.Attach(DetermineMainAdapter());
	if(mainAdapter == nullptr){
		Debug::Log(SID("RENDER"), "Failed to determine main DXGI adapter! You may need to update your graphics card drivers", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	D3D_FEATURE_LEVEL maxFeatureLevel = GetMaxFeatureLevel(mainAdapter.Get());
	GADGET_BASIC_ASSERT(maxFeatureLevel >= minimumFeatureLevel);
	if(maxFeatureLevel < minimumFeatureLevel){
		Debug::Log(SID("RENDER"), "Max supported feature level is too low, something is wrong with Win32_DX12_Renderer::DetermineMainAdapter. Also, you may need to update your graphics card drivers", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	if(FAILED(D3D12CreateDevice(mainAdapter.Get(), maxFeatureLevel, IID_PPV_ARGS(&mainDevice))) || !mainDevice){
		Debug::Log(SID("RENDER"), "Failed to create D3D12Device!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	mainDevice->SetName(L"MAIN DEVICE");
	Debug::Log(SID("RENDER"), "Created new D3D12 device with name [MAIN DEVICE]", Debug::Info, __FILE__, __LINE__);
	return ErrorCode::OK;
}

ErrorCode DX12::CreateCommandList(){
	gfxCommand = new DX12_Command(DX12::MainDevice(), D3D12_COMMAND_LIST_TYPE_DIRECT);
	if(gfxCommand == nullptr){
		return ErrorCode::D3D12_Error;
	}

	return ErrorCode::OK;
}

ErrorCode DX12::InitializeDescriptorHeaps(){
	bool br = rtvDescriptorHeap.Initialize(mainDevice.Get(), 512, false); //TODO - These capacity numbers are somewhat arbitrary - makes these configurable?
	if(br == false || rtvDescriptorHeap.Heap() == nullptr){
		Debug::Log(SID("RENDER"), "Failed to initialize rtvDescriptorHeap!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}
	rtvDescriptorHeap.Heap()->SetName(L"RTV DescriptorHeap");

	br = dsvDescriptorHeap.Initialize(mainDevice.Get(), 512, false);
	if(br == false || dsvDescriptorHeap.Heap() == nullptr){
		Debug::Log(SID("RENDER"), "Failed to initialize dsvDescriptorHeap!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}
	dsvDescriptorHeap.Heap()->SetName(L"DSV DescriptorHeap");

	br = srvDescriptorHeap.Initialize(mainDevice.Get(), 4096, true);
	if(br == false || srvDescriptorHeap.Heap() == nullptr){
		Debug::Log(SID("RENDER"), "Failed to initialize srvDescriptorHeap!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}
	srvDescriptorHeap.Heap()->SetName(L"SRV DescriptorHeap");

	br = uavDescriptorHeap.Initialize(mainDevice.Get(), 512, false);
	if(br == false || uavDescriptorHeap.Heap() == nullptr){
		Debug::Log(SID("RENDER"), "Failed to initialize uavDescriptorHeap!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}
	uavDescriptorHeap.Heap()->SetName(L"UAV DescriptorHeap");

	return ErrorCode::OK;
}

ErrorCode DX12::BreakOnWarningsAndErrors(bool enabled_){
	GADGET_BASIC_ASSERT(mainDevice != nullptr);
	if(mainDevice == nullptr){
		Debug::Log(SID("RENDER"), "", Debug::Warning, __FILE__, __LINE__);
		return ErrorCode::Invalid_State;
	}

	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
	if(FAILED(mainDevice->QueryInterface(IID_PPV_ARGS(infoQueue.ReleaseAndGetAddressOf())))){
		Debug::Log(SID("RENDER"), "Failed to query D3D12Device!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	if(FAILED(infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, enabled_))){
		Debug::Log(SID("RENDER"), "Debug break on DX12 corruption could not be set!", Debug::Warning, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	if(FAILED(infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, enabled_))){
		Debug::Log(SID("RENDER"), "Debug break on DX12 error could not be set!", Debug::Warning, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	if(FAILED(infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, enabled_))){
		Debug::Log(SID("RENDER"), "Debug break on DX12 warning could not be set!", Debug::Warning, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	return ErrorCode::OK;
}

bool DX12::IsInitialized(){ return mainDevice != nullptr && gfxCommand != nullptr; }

ID3D12_Device* const DX12::MainDevice(){ return mainDevice.Get(); }

uint32_t DX12::CurrentFrameIndex(){
	if(!IsInitialized()){
		return 0;
	}

	return gfxCommand->CurrentFrameIndex();
}

void DX12::CreateSwapChainForSurface(DX12_RenderSurface* surface_){
	GADGET_BASIC_ASSERT(surface_ != nullptr);
	GADGET_BASIC_ASSERT(gfxCommand != nullptr);

	surface_->CreateSwapChain(dxgiFactory.Get(), gfxCommand->CommandQueue());
}

void DX12::ResizeSurface(DX12_RenderSurface* surface_, int width_, int height_){
	GADGET_BASIC_ASSERT(surface_ != nullptr);
	GADGET_BASIC_ASSERT(gfxCommand != nullptr);

	gfxCommand->Flush();
	surface_->SetSize(ScreenCoordinate(width_, height_));
}

void DX12::DeferredRelease(IUnknown* resource_){
	std::lock_guard lock{ deferredReleaseMutex };
	deferredReleases[CurrentFrameIndex()].push_back(resource_);
	SetDeferredReleaseFlag();
}

uint32_t DX12::GetDeferredReleaseFlag(){
	return deferredReleaseFlag[CurrentFrameIndex()];
}

void DX12::SetDeferredReleaseFlag(){
	deferredReleaseFlag[CurrentFrameIndex()] = 1;
}

void DX12::ProcessDeferredReleases(uint32_t frameIndex_){
	if(deferredReleaseFlag[frameIndex_] == 0){
		return;
	}

	std::lock_guard lock{ deferredReleaseMutex };

	deferredReleaseFlag[frameIndex_] = 0;

	rtvDescriptorHeap.ProcessDeferredFree(frameIndex_);
	dsvDescriptorHeap.ProcessDeferredFree(frameIndex_);
	srvDescriptorHeap.ProcessDeferredFree(frameIndex_);
	uavDescriptorHeap.ProcessDeferredFree(frameIndex_);

	for(const auto& resource : deferredReleases[frameIndex_]){
		resource->Release();
	}
	deferredReleases[frameIndex_].clear();
}

void DX12::ProcessAllDeferredReleases(){
	for(uint32_t i = 0; i < FrameBufferCount; i++){
		ProcessDeferredReleases(i);
	}
}

ErrorCode DX12::DebugShutdown(){
	if(!debugLayerEnabled){
		return ErrorCode::OK;
	}

#ifdef GADGET_DEBUG
	auto err = BreakOnWarningsAndErrors(false);
	if(err != ErrorCode::OK){
		return err;
	}

	Microsoft::WRL::ComPtr<ID3D12_DebugDevice> debugDevice;
	if(FAILED(mainDevice->QueryInterface(IID_PPV_ARGS(debugDevice.ReleaseAndGetAddressOf())))){
		Debug::Log(SID("RENDER"), "Failed to query D3D12 debug device!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	if(mainDevice){
		mainDevice.Reset();
	}

	//Check for leaks
	if(FAILED(debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL))){
		Debug::Log(SID("RENDER"), "Failed to report live objects from D3D12 debug device!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

#endif //GADGET_DEBUG

	return ErrorCode::OK;
}

IDXGI_Adapter* DX12::DetermineMainAdapter(){
	IDXGI_Adapter* adapter = nullptr;

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

D3D_FEATURE_LEVEL DX12::GetMaxFeatureLevel(IDXGI_Adapter* adapter_){
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
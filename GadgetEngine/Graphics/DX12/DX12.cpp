#include "DX12.h"

#include <wrl.h>

#include "Debug.h"
#include "Graphics/DX12/DX12_Command.h"
#include "Graphics/DX12/DX12_DescriptorHeap.h"
#include "Graphics/DX12/DX12_RenderSurface.h"

using namespace Gadget;
using Microsoft::WRL::ComPtr;

std::unique_ptr<DX12> DX12::instance = nullptr;
DWORD DX12::callbackCookie = 0;

DX12::DX12(const DX12_StartupOptions& options_) :	minimumFeatureLevel(D3D_FEATURE_LEVEL_12_0), dxgiFactory(nullptr), mainDevice(nullptr), gfxCommand(nullptr), resourceBarriers(),
													rtvDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), dsvDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV),
													srvDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), uavDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),
													deferredReleases(FrameBufferCount), deferredReleaseMutex(){
	GADGET_BASIC_ASSERT(deferredReleases.size() == FrameBufferCount);

	if(options_.requireDXR){
		minimumFeatureLevel = D3D_FEATURE_LEVEL_12_1;
	}

	auto err = EnableDebugLayer(options_.gpuBasedValidation);
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "[DX12] Could not enable debug layer!", err, __FILE__, __LINE__);
	}

	err = CreateDevice(options_.dxgiFactoryFlags, options_.requireDXR);
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "[DX12] Could not create device!", err, __FILE__, __LINE__);
	}

	err = RegisterDebugCallback();
	if(err != ErrorCode::OK){
		GADGET_LOG_WARNING(SID("RENDER"), "D3D12 messages will not be logged to the console!");
	}

	err = BreakOnWarningsAndErrors(options_.breakOnWarnings, options_.breakOnErrors);
	if(err != ErrorCode::OK){
		Debug::Log(SID("RENDER"), "DX12 code will not break on warnings or errors!", Debug::Error, __FILE__, __LINE__);
	}

	err = CreateCommandList(options_.closeCommandListOnInit);
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "[DX12] Could not create command list!", err, __FILE__, __LINE__);
	}
	
	err = InitializeDescriptorHeaps();
	if(err != ErrorCode::OK){
		Debug::ThrowFatalError(SID("RENDER"), "[DX12] Could not initialize descriptor heaps!", err, __FILE__, __LINE__);
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
			//We should try to shutdown anyway even if something goes wrong in PreShutdown
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

bool DX12::IsInstanceInitialized(){
	return instance != nullptr && instance->IsInitialized();
}

ErrorCode DX12::EnableDebugLayer([[maybe_unused]] bool gpuValidation_){
#ifdef GADGET_DEBUG
	GADGET_BASIC_ASSERT(mainDevice == nullptr);
	if(mainDevice != nullptr){
		Debug::Log(SID("RENDER"), "[DX12] Debug layer cannot be enabled after the device has been created", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::Invalid_State;
	}

	Microsoft::WRL::ComPtr<ID3D12_Debug> debugInterface;
	if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugInterface.ReleaseAndGetAddressOf())))){
		debugInterface->EnableDebugLayer();
		debugInterface->SetEnableGPUBasedValidation(gpuValidation_);
	}else{
		Debug::Log(SID("RENDER"), "Failed to get D3D12 Debug Interface!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}
#endif //GADGET_DEBUG

	return ErrorCode::OK;
}

ErrorCode DX12::RegisterDebugCallback(){
#ifdef GADGET_DEBUG
	GADGET_BASIC_ASSERT(mainDevice != nullptr);
	if(mainDevice == nullptr){
		GADGET_LOG_WARNING(SID("RENDER"), "Cannot register debug callback before the main device has been created!");
		return ErrorCode::Invalid_State;
	}

	//Unregister the existing callback if one's already registered
	//We could just do nothing and return here, but this feels more correct
	if(callbackCookie != 0){
		auto err = UnregisterDebugCallback();
		if(err != ErrorCode::OK){
			GADGET_LOG_ERROR(SID("RENDER"), "D3D12 had an existing debug callback set, but it could not be unregistered.");
			return err;
		}
	}

	Microsoft::WRL::ComPtr<ID3D12_InfoQueue> infoQueue;
	if(FAILED(mainDevice.As(&infoQueue)) || infoQueue == nullptr){
		GADGET_LOG_ERROR(SID("RENDER"), "D3D12Device could not be queried as ID3D12InfoQueue! D3D12 messages will not be debug logged.");
		return ErrorCode::D3D12_Error;
	}
	
	HRESULT hr = infoQueue->RegisterMessageCallback(&DebugMessageCallback, D3D12_MESSAGE_CALLBACK_FLAG_NONE, nullptr, &callbackCookie);
	if(FAILED(hr) || callbackCookie == 0){
		GADGET_LOG_ERROR(SID("RENDER"), "Could not register message callback. D3D12 messages will not be debug logged.");
		return ErrorCode::D3D12_Error;
	}
#endif //GADGET_DEBUG

	return ErrorCode::OK;
}

ErrorCode DX12::UnregisterDebugCallback(){
#ifdef GADGET_DEBUG
	if(callbackCookie == 0){
		return ErrorCode::OK; //We never registered the callback
	}

	GADGET_BASIC_ASSERT(mainDevice != nullptr);
	if(mainDevice == nullptr){
		GADGET_LOG_WARNING(SID("RENDER"), "Cannot unregister debug callback if the main device is not set!");
		return ErrorCode::OK;
	}

	Microsoft::WRL::ComPtr<ID3D12_InfoQueue> infoQueue;
	if(FAILED(mainDevice.As(&infoQueue)) || infoQueue == nullptr){
		GADGET_LOG_ERROR(SID("RENDER"), "D3D12Device could not be queried as ID3D12InfoQueue! We won't be able to unregister the debug message callback!");
		return ErrorCode::D3D12_Error;
	}
	
	HRESULT hr = infoQueue->UnregisterMessageCallback(callbackCookie);
	if(FAILED(hr)){
		GADGET_LOG_ERROR(SID("RENDER"), "Could not unregister message callback.");
		//Oh well
	}

	callbackCookie = 0;
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
		Debug::Log(SID("RENDER"), "An error occurred in DX12::DebugShutdown! Error Code: " + std::to_string((uint32_t)err), Debug::Warning, __FILE__, __LINE__);
	}

	#endif // GADGET_DEBUG

	mainDevice.Reset();

	return ErrorCode::OK;
}

ErrorCode DX12::CreateDevice(uint32_t dxgiFactoryFlags_, bool requireDXR_){
	if(FAILED(CreateDXGIFactory2(dxgiFactoryFlags_, IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf())))){
		Debug::Log(SID("RENDER"), "Failed to create DXGI Factory!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	Microsoft::WRL::ComPtr<IDXGI_Adapter> mainAdapter;
	mainAdapter.Attach(DetermineMainAdapter(requireDXR_));
	if(mainAdapter == nullptr){
		Debug::Log(SID("RENDER"), "Failed to determine main DXGI adapter! You may need to update your graphics card drivers", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_NoValidAdapter;
	}

	D3D_FEATURE_LEVEL maxFeatureLevel = GetMaxFeatureLevel(mainAdapter.Get());
	GADGET_BASIC_ASSERT(maxFeatureLevel >= minimumFeatureLevel);
	if(maxFeatureLevel < minimumFeatureLevel){
		Debug::Log(SID("RENDER"), "Max supported feature level is too low, something is wrong with the DetermineMainAdapter logic. You also may need to update your graphics card drivers", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_BadFeatureLevel;
	}

	if(FAILED(D3D12CreateDevice(mainAdapter.Get(), maxFeatureLevel, IID_PPV_ARGS(&mainDevice))) || !mainDevice){
		Debug::Log(SID("RENDER"), "Failed to create D3D12Device!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	mainDevice->SetName(L"MAIN DEVICE");
	Debug::Log(SID("RENDER"), "Created new D3D12 device with name [MAIN DEVICE]", Debug::Info, __FILE__, __LINE__);
	return ErrorCode::OK;
}

ErrorCode DX12::CreateCommandList(bool closeCommandListOnInit_){
	GADGET_BASIC_ASSERT(mainDevice != nullptr);
	if(mainDevice == nullptr){
		Debug::Log(SID("RENDER"), "[DX12] Tried to create command list, but the device was not initialized", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::Invalid_State;
	}

	gfxCommand = new DX12_Command(mainDevice.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
	if(gfxCommand == nullptr){
		return ErrorCode::Constructor_Failed;
	}

	if(closeCommandListOnInit_){
		auto err = gfxCommand->CloseList();
		if(err != ErrorCode::OK){
			return err;
		}
	}

	return ErrorCode::OK;
}

ErrorCode DX12::InitializeDescriptorHeaps(){
	GADGET_BASIC_ASSERT(mainDevice != nullptr);
	if(mainDevice == nullptr){
		Debug::Log(SID("RENDER"), "Tried to initialize descriptor heaps, but the device was not initialized", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::Invalid_State;
	}

	auto err = rtvDescriptorHeap.Initialize(mainDevice.Get(), 512, false); //TODO - These capacity numbers are somewhat arbitrary - makes these configurable?
	if(err != ErrorCode::OK || rtvDescriptorHeap.Heap() == nullptr){
		Debug::Log(SID("RENDER"), "Failed to initialize rtvDescriptorHeap!", Debug::Error, __FILE__, __LINE__);
		return err;
	}
	rtvDescriptorHeap.Heap()->SetName(L"RTV DescriptorHeap");

	err = dsvDescriptorHeap.Initialize(mainDevice.Get(), 512, false);
	if(err != ErrorCode::OK || dsvDescriptorHeap.Heap() == nullptr){
		Debug::Log(SID("RENDER"), "Failed to initialize dsvDescriptorHeap!", Debug::Error, __FILE__, __LINE__);
		return err;
	}
	dsvDescriptorHeap.Heap()->SetName(L"DSV DescriptorHeap");

	err = srvDescriptorHeap.Initialize(mainDevice.Get(), 4096, true);
	if(err != ErrorCode::OK || srvDescriptorHeap.Heap() == nullptr){
		Debug::Log(SID("RENDER"), "Failed to initialize srvDescriptorHeap!", Debug::Error, __FILE__, __LINE__);
		return err;
	}
	srvDescriptorHeap.Heap()->SetName(L"SRV DescriptorHeap");

	err = uavDescriptorHeap.Initialize(mainDevice.Get(), 512, false);
	if(err != ErrorCode::OK || uavDescriptorHeap.Heap() == nullptr){
		Debug::Log(SID("RENDER"), "Failed to initialize uavDescriptorHeap!", Debug::Error, __FILE__, __LINE__);
		return err;
	}
	uavDescriptorHeap.Heap()->SetName(L"UAV DescriptorHeap");

	return ErrorCode::OK;
}

ErrorCode DX12::BreakOnWarningsAndErrors([[maybe_unused]] bool breakOnWarnings_, [[maybe_unused]] bool breakOnErrors_){
#ifdef GADGET_DEBUG
	GADGET_BASIC_ASSERT(mainDevice != nullptr);
	if(mainDevice == nullptr){
		Debug::Log(SID("RENDER"), "BreakOnWarningsAndErrors requires the device to be initialized", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::Invalid_State;
	}

	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
	if(FAILED(mainDevice.As(&infoQueue))){
		Debug::Log(SID("RENDER"), "Failed to query D3D12Device!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	if(FAILED(infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, breakOnErrors_))){
		Debug::Log(SID("RENDER"), "Debug break on DX12 corruption could not be set!", Debug::Warning, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	if(FAILED(infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, breakOnErrors_))){
		Debug::Log(SID("RENDER"), "Debug break on DX12 error could not be set!", Debug::Warning, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	if(FAILED(infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, breakOnWarnings_))){
		Debug::Log(SID("RENDER"), "Debug break on DX12 warning could not be set!", Debug::Warning, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}
#endif //GADGET_DEBUG

	return ErrorCode::OK;
}

bool DX12::IsInitialized() const{ return mainDevice != nullptr && gfxCommand != nullptr; }

ID3D12_Device* const DX12::MainDevice() const{ return mainDevice.Get(); }

uint32_t DX12::CurrentFrameIndex() const{
	if(!IsInitialized()){
		return 0;
	}

	return gfxCommand->CurrentFrameIndex();
}

ErrorCode DX12::FlushAndSetFrameIndex(uint32_t frameIndex_){
	if(!IsInitialized()){
		return ErrorCode::Invalid_State;
	}

	auto err = gfxCommand->Flush();
	if(err != ErrorCode::OK){
		GADGET_LOG_ERROR(SID("RENDER"), "Could not flush commands before setting the new frame index!");
		return err;
	}

	gfxCommand->SetCurrentFrameIndex(frameIndex_);
	return ErrorCode::OK;
}

ErrorCode DX12::CreateSwapChainForSurface(DX12_RenderSurface* surface_){
	GADGET_BASIC_ASSERT(surface_ != nullptr);
	GADGET_BASIC_ASSERT(dxgiFactory != nullptr);
	GADGET_BASIC_ASSERT(gfxCommand != nullptr);

	if(surface_ == nullptr){
		Debug::Log(SID("RENDER"), "Tried to create a swapchain for a null surface", Debug::Warning, __FILE__, __LINE__);
		return ErrorCode::OK; //Could return Invalid_Args, but since we're only logging a warning we should treat this as a non-fail state
	}

	if(dxgiFactory == nullptr || gfxCommand == nullptr){
		Debug::Log(SID("RENDER"), "Tried to create a swapchain while DX12 was not fully initialized", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::Invalid_State;
	}

	return surface_->CreateSwapChain(dxgiFactory.Get(), gfxCommand->CommandQueue());
}

ErrorCode DX12::ResizeSurface(DX12_RenderSurface* surface_, int width_, int height_){
	GADGET_BASIC_ASSERT(surface_ != nullptr);
	GADGET_BASIC_ASSERT(gfxCommand != nullptr);

	ErrorCode err = gfxCommand->Flush();
	if(err != ErrorCode::OK){
		Debug::Log(SID("RENDER"), "Could not flush command list", Debug::Error, __FILE__, __LINE__);
		return err;
	}

	return surface_->SetSize(ScreenCoordinate(width_, height_));
}

void DX12::DeferredRelease(IUnknown* resource_){
	GADGET_BASIC_ASSERT(CurrentFrameIndex() < deferredReleases.size());
	GADGET_BASIC_ASSERT(deferredReleases.size() == FrameBufferCount);

	std::lock_guard lock{ deferredReleaseMutex };

	deferredReleases[CurrentFrameIndex()].resources.push_back(resource_);
	SetDeferredReleaseFlag();
}

uint32_t DX12::GetDeferredReleaseFlag(){
	return deferredReleases[CurrentFrameIndex()].flag;
}

void DX12::SetDeferredReleaseFlag(){
	deferredReleases[CurrentFrameIndex()].flag = 1;
}

void DX12::ProcessDeferredReleases(uint32_t frameIndex_){
	if(deferredReleases[frameIndex_].flag == 0){
		return;
	}

	std::lock_guard lock{ deferredReleaseMutex };

	deferredReleases[frameIndex_].flag = 0;

	rtvDescriptorHeap.ProcessDeferredFree(frameIndex_);
	dsvDescriptorHeap.ProcessDeferredFree(frameIndex_);
	srvDescriptorHeap.ProcessDeferredFree(frameIndex_);
	uavDescriptorHeap.ProcessDeferredFree(frameIndex_);

	for(const auto& resource : deferredReleases[frameIndex_].resources){
		resource->Release();
	}
	deferredReleases[frameIndex_].resources.clear();
}

void DX12::ProcessDeferredReleasesForCurrentFrame(){
	ProcessDeferredReleases(CurrentFrameIndex());
}

void DX12::ProcessAllDeferredReleases(){
	for(uint32_t i = 0; i < FrameBufferCount; i++){
		ProcessDeferredReleases(i);
	}
}

ErrorCode DX12::DebugShutdown(){
#ifdef GADGET_DEBUG
	auto err = BreakOnWarningsAndErrors(false, false);
	if(err != ErrorCode::OK){
		return err;
	}

	Microsoft::WRL::ComPtr<ID3D12_DebugDevice> debugDevice;
	if(FAILED(mainDevice.As(&debugDevice))){
		Debug::Log(SID("RENDER"), "Failed to query D3D12 debug device!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	mainDevice.Reset();

	//Check for leaks
	if(FAILED(debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL))){
		Debug::Log(SID("RENDER"), "Failed to report live objects from D3D12 debug device!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::D3D12_Error;
	}

	debugDevice.Reset();

#endif //GADGET_DEBUG

	return ErrorCode::OK;
}

IDXGI_Adapter* DX12::DetermineMainAdapter(bool requireDXR_){
	IDXGI_Adapter* adapter = nullptr;

	for(int i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND; i++){
		ComPtr<ID3D12_Device> tempDevice;
		if(SUCCEEDED(D3D12CreateDevice(adapter, minimumFeatureLevel, IID_PPV_ARGS(tempDevice.ReleaseAndGetAddressOf())))){
			//If we need DXR, make sure the device actually supports it
			if(!requireDXR_ || DoesDeviceSupportRaytracing(tempDevice.Get())){
				return adapter;
			}
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

	constexpr D3D_FEATURE_LEVEL featureLevels[5]{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_2,
	};

	D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelInfo{};
	featureLevelInfo.NumFeatureLevels = static_cast<uint32_t>(std::size(featureLevels));
	featureLevelInfo.pFeatureLevelsRequested = featureLevels;

	Microsoft::WRL::ComPtr<ID3D12Device> device;
	HRESULT result = D3D12CreateDevice(adapter_, minimumFeatureLevel, IID_PPV_ARGS(device.ReleaseAndGetAddressOf()));
	if(FAILED(result) || !device){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to create D3D12 device at requested minimum feature level!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}

	result = device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelInfo, sizeof(featureLevelInfo));
	if(FAILED(result)){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to check feature level support on D3D12 device!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}

	return featureLevelInfo.MaxSupportedFeatureLevel;
}

bool DX12::DoesDeviceSupportRaytracing(ID3D12_Device* device_) const{
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options5 = {};
	if(FAILED(device_->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options5, sizeof(options5)))){
		GADGET_LOG_ERROR(SID("RENDER"), "");
		return false;
	}

	return options5.RaytracingTier >= D3D12_RAYTRACING_TIER_1_0;
}

void DX12::DebugMessageCallback([[maybe_unused]] D3D12_MESSAGE_CATEGORY category_, D3D12_MESSAGE_SEVERITY severity_, [[maybe_unused]] D3D12_MESSAGE_ID id_, LPCSTR pDescription_, [[maybe_unused]] void* pContext_){
	//Note 1 -	We cannot guarantee that this is being called on the render thread (or any particular thread)
	//Note 2 -	We cannot guarantee anything about the internal D3D runtime state when this gets called.
	//			Calling D3D functions here could cause deadlocks, crashes, or undefined behaviour

	switch(severity_){
		case D3D12_MESSAGE_SEVERITY_CORRUPTION: [[fallthrough]];
		case D3D12_MESSAGE_SEVERITY_ERROR:
			Debug::Log(SID("D3D12"), pDescription_, Debug::Error); //Leaving out file and line since they won't mean anything here
			break;
		case D3D12_MESSAGE_SEVERITY_WARNING:
			Debug::Log(SID("D3D12"), pDescription_, Debug::Warning); //Leaving out file and line since they won't mean anything here
			break;
		case D3D12_MESSAGE_SEVERITY_INFO:
			Debug::Log(SID("D3D12"), pDescription_, Debug::Info); //Leaving out file and line since they won't mean anything here
			break;
		case D3D12_MESSAGE_SEVERITY_MESSAGE:
			Debug::Log(SID("D3D12"), pDescription_, Debug::Verbose); //Leaving out file and line since they won't mean anything here
			break;
		default:
			break;
	}
}
#include "DXR_PipelineStateObject.h"

#include "Graphics/DX12/DX12.h"
#include "Graphics/DX12/DX12_ShaderHandler.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/RaytracingPipelineGenerator.h"
#include "Graphics/DX12/DXR/nv_helpers_dx12/RootSignatureGenerator.h"

using namespace Gadget;

DXR_PipelineStateObject::DXR_PipelineStateObject(){
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());

	nv_helpers_dx12::RayTracingPipelineGenerator pipeline(DX12::GetInstance().MainDevice());

	rayGenLibrary = DX12_ShaderHandler::GetEngineShader(EngineShader::ID::RayGen_Lib);
	missLibrary = DX12_ShaderHandler::GetEngineShader(EngineShader::ID::Miss_Lib);
	hitLibrary = DX12_ShaderHandler::GetEngineShader(EngineShader::ID::Hit_Lib);

	GADGET_BASIC_ASSERT(rayGenLibrary.BytecodeLength > 0 && rayGenLibrary.pShaderBytecode != nullptr);
	GADGET_BASIC_ASSERT(missLibrary.BytecodeLength > 0 && missLibrary.pShaderBytecode != nullptr);
	GADGET_BASIC_ASSERT(hitLibrary.BytecodeLength > 0 && hitLibrary.pShaderBytecode != nullptr);

	pipeline.AddLibrary(rayGenLibrary, { L"RayGen" });
	pipeline.AddLibrary(missLibrary, { L"Miss" });
	pipeline.AddLibrary(hitLibrary, { L"ClosestHit", L"PlaneClosestHit" });

	rayGenSignature.Attach(CreateRayGenSignature());
	missSignature.Attach(CreateMissSignature());
	hitSignature.Attach(CreateHitSignature());

	GADGET_BASIC_ASSERT(rayGenSignature != nullptr);
	GADGET_BASIC_ASSERT(missSignature != nullptr);
	GADGET_BASIC_ASSERT(hitSignature != nullptr);

	pipeline.AddHitGroup(L"HitGroup", L"ClosestHit");
	pipeline.AddHitGroup(L"PlaneHitGroup", L"PlaneClosestHit");

	pipeline.AddRootSignatureAssociation(rayGenSignature.Get(), { L"RayGen" });
	pipeline.AddRootSignatureAssociation(missSignature.Get(), { L"Miss" });
	pipeline.AddRootSignatureAssociation(hitSignature.Get(), { L"HitGroup", L"PlaneHitGroup" });

	pipeline.SetMaxPayloadSize(4 * sizeof(float));
	pipeline.SetMaxAttributeSize(2 * sizeof(float));
	pipeline.SetMaxRecursionDepth(1);

	rtStateObject.Attach(pipeline.Generate());
	if(rtStateObject == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not generate raytracing pipeline!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
	rtStateObject->SetName(L"Raytracing Pipeline State Object");

	HRESULT hr = rtStateObject->QueryInterface(IID_PPV_ARGS(rtStateObjectProperties.ReleaseAndGetAddressOf()));
	if(FAILED(hr) || rtStateObjectProperties == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not query interface for rtStateObject!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}
}

ID3D12RootSignature* DXR_PipelineStateObject::CreateRayGenSignature(){
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());

	nv_helpers_dx12::RootSignatureGenerator rsg;
	rsg.AddHeapRangesParameter(
		{
			{ 0, 1, 0, D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0 }, //Output buffer
			{ 0, 1, 0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1 }, //Top-level acceleration structure
			{ 0, 1, 0, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 2 }, //Camera parameters
		}
	);

	ID3D12RootSignature* rootSig = rsg.Generate(DX12::GetInstance().MainDevice(), true);
	if(rootSig == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create ray generation signature!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}

	rootSig->SetName(L"RayGen Root Signature");
	return rootSig;
}

ID3D12RootSignature* DXR_PipelineStateObject::CreateMissSignature(){
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());

	nv_helpers_dx12::RootSignatureGenerator rsg;
	ID3D12RootSignature* rootSig = rsg.Generate(DX12::GetInstance().MainDevice(), true);
	if(rootSig == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create miss signature!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}

	rootSig->SetName(L"Miss Root Signature");
	return rootSig;
}

ID3D12RootSignature* DXR_PipelineStateObject::CreateHitSignature(){
	GADGET_BASIC_ASSERT(DX12::IsInstanceInitialized());

	nv_helpers_dx12::RootSignatureGenerator rsg;
	rsg.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_SRV, 0); //Vertices
	rsg.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_SRV, 1); //Indices
	rsg.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 0); //Colors

	ID3D12RootSignature* rootSig = rsg.Generate(DX12::GetInstance().MainDevice(), true);
	if(rootSig == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Could not create hit signature!", ErrorCode::D3D12_Error, __FILE__, __LINE__);
	}

	rootSig->SetName(L"Hit Root Signature");
	return rootSig;
}

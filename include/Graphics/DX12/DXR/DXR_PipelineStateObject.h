#ifndef GADGET_DXR_PIPELINE_STATE_OBJECT_H
#define GADGET_DXR_PIPELINE_STATE_OBJECT_H

#include <d3d12.h>
#include <wrl/client.h>

namespace Gadget{
	class DXR_PipelineStateObject{
	public:
		DXR_PipelineStateObject();

		ID3D12StateObject* StateObject(){ return rtStateObject.Get(); }
		ID3D12StateObjectProperties* Properties(){ return rtStateObjectProperties.Get(); }

		LPCWSTR HitGroupName(size_t index_);

	private:
		D3D12_SHADER_BYTECODE rayGenLibrary;
		D3D12_SHADER_BYTECODE hitLibrary;
		D3D12_SHADER_BYTECODE missLibrary;

		Microsoft::WRL::ComPtr<ID3D12RootSignature> rayGenSignature;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> hitSignature;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> missSignature;

		Microsoft::WRL::ComPtr<ID3D12StateObject> rtStateObject;
		Microsoft::WRL::ComPtr<ID3D12StateObjectProperties> rtStateObjectProperties;

		ID3D12RootSignature* CreateRayGenSignature();
		ID3D12RootSignature* CreateMissSignature();
		ID3D12RootSignature* CreateHitSignature();
	};
}

#endif //!GADGET_DXR_PIPELINE_STATE_OBJECT_H

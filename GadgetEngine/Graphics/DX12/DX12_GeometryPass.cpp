#include "DX12_GeometryPass.h"

using namespace Gadget;

DX12_RenderTextureInfo* DX12_GeometryPass::mainBuffer = nullptr;
DX12_DepthBufferTextureInfo* DX12_GeometryPass::depthBuffer = nullptr;
ScreenCoordinate DX12_GeometryPass::size = initialSize;

bool DX12_GeometryPass::Initialize(){
	return CreateBuffers(size);
}

void DX12_GeometryPass::Shutdown(){
	if(mainBuffer != nullptr){
		delete mainBuffer;
		mainBuffer = nullptr;
	}

	if(depthBuffer != nullptr){
		delete depthBuffer;
		depthBuffer = nullptr;
	}

	size = initialSize;
}

void DX12_GeometryPass::OnResize(const ScreenCoordinate& newSize_){
	if(newSize_ == size){
		return;
	}

	size = newSize_;
	CreateBuffers(newSize_);
}

bool DX12_GeometryPass::CreateBuffers(const ScreenCoordinate& size_){
	GADGET_BASIC_ASSERT(size_.x > 0);
	GADGET_BASIC_ASSERT(size_.y > 0);

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
	info.initialState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	info.clearValue.Format = desc.Format;
	info.clearValue.Color[0] = 0.0f; //TODO - Renderer::SetClearColor should affect this somehow
	info.clearValue.Color[1] = 0.0f;
	info.clearValue.Color[2] = 0.0f;
	info.clearValue.Color[3] = 0.0f;

	mainBuffer = new DX12_RenderTextureInfo(info);
	GADGET_BASIC_ASSERT(mainBuffer != nullptr);
	GADGET_BASIC_ASSERT(mainBuffer->GetResource() != nullptr);
	mainBuffer->GetResource()->SetName(L"Geometry Pass Main Buffer");

	//Depth Buffer
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	desc.Format = depthBufferFormat;
	desc.MipLevels = 1;

	DX12_TextureInitInfo info2;
	info2.resourceDesc = &desc;
	info.initialState = D3D12_RESOURCE_STATE_DEPTH_READ | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	info.clearValue.Format = desc.Format;
	info.clearValue.DepthStencil.Depth = 0.0f;
	info.clearValue.DepthStencil.Stencil = 0;

	depthBuffer = new DX12_DepthBufferTextureInfo(info);
	GADGET_BASIC_ASSERT(depthBuffer != nullptr);
	GADGET_BASIC_ASSERT(depthBuffer->GetResource() != nullptr);
	depthBuffer->GetResource()->SetName(L"Geometry Pass Depth Buffer");

	return mainBuffer->GetResource() != nullptr && depthBuffer->GetResource() != nullptr;
}
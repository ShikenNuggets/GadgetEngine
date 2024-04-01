#include "DX12_GeometryPass.h"

using namespace Gadget;

DX12_RenderTextureInfo* DX12_GeometryPass::mainBuffer = nullptr;
DX12_DepthBufferTextureInfo* DX12_GeometryPass::depthBuffer = nullptr;
ScreenCoordinate DX12_GeometryPass::size = ScreenCoordinate(0, 0);
Color DX12_GeometryPass::clearColor = Color::Black();

bool DX12_GeometryPass::Initialize(const ScreenCoordinate& size_, const Color& clearColor_){
	size = size_;
	clearColor = clearColor_;
	return CreateBuffers(size, clearColor);
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

	size = ScreenCoordinate(0, 0);
	clearColor = Color::Black();
}

void DX12_GeometryPass::SetClearColor(const Color& color_){
	if(color_ == clearColor){
		return;
	}

	clearColor = color_;
	CreateBuffers(size, color_); //This feels like it could be more efficient, but we generally only set the clear color once per session so it's probably fine
}

void DX12_GeometryPass::OnResize(const ScreenCoordinate& newSize_){
	if(newSize_ == size){
		return;
	}

	size = newSize_;
	CreateBuffers(newSize_, clearColor);
}

bool DX12_GeometryPass::CreateBuffers(const ScreenCoordinate& size_, const Color& clearColor_){
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
	info.clearValue.Color[0] = clearColor_.r;
	info.clearValue.Color[1] = clearColor_.g;
	info.clearValue.Color[2] = clearColor_.b;
	info.clearValue.Color[3] = clearColor_.a;

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
#include "Win32_DX12_Renderer.h"

#include "Win32_Window.h"

using namespace Gadget;

Win32_DX12_Renderer::Win32_DX12_Renderer(int w_, int h_, int x_, int y_) : Renderer(API::DX12){
	window = std::make_unique<Win32_Window>(w_, h_, x_, y_);

	Win32_Window* win32Window = dynamic_cast<Win32_Window*>(window.get());
	GADGET_ASSERT(win32Window != nullptr, "Win32 Renderer requires a Win32 window!");
}

Win32_DX12_Renderer::~Win32_DX12_Renderer(){
	window.reset();
}

void Win32_DX12_Renderer::PostInit(){
	Renderer::PostInit();
}

void Win32_DX12_Renderer::Render(const Scene* scene_){
	//Do this only at the end
	window.get()->SwapBuffers();
}

void Win32_DX12_Renderer::ClearScreen(){
}

void Win32_DX12_Renderer::SetClearColor(const Color& color_){
}

void Win32_DX12_Renderer::SetViewportRect(const Rect& rect_){
	GADGET_ASSERT(rect_.x >= 0.0f && rect_.x <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.y >= 0.0f && rect_.y <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.w >= 0.0f && rect_.w <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.h >= 0.0f && rect_.h <= 1.0f, "Tried to set invalid viewport rect!");
}

void Win32_DX12_Renderer::OnResize(int width_, int height_){
	if(!postInitComplete){
		return; //New size will be handled correctly when we finish initializing
	}
}

void Win32_DX12_Renderer::SetWindingOrder(WindingOrder order_){
	Renderer::SetWindingOrder(order_);
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
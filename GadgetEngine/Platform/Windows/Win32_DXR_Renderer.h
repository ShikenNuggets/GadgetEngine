#ifndef GADGET_WIN32_DXR_RENDERER_H
#define GADGET_WIN32_DXR_RENDERER_H

#include "Graphics/Renderer.h"
#include "Graphics/DX12/DX12_RenderSurface.h"
#include "Graphics/DX12/DX12_TextureInfo.h"
#include "Graphics/DX12/DXR/DXR_MeshInfo.h"

namespace Gadget{
	class DX12;
	class DXR;

	class Win32_DXR_Renderer : public Renderer{
	public:
		Win32_DXR_Renderer(int w_ = 800, int h_ = 600, int x_ = 0, int y_ = 0);
		virtual ~Win32_DXR_Renderer() override;

		virtual void PostInit() override;

		virtual void Render(const Scene* scene_) override;
		virtual void ClearScreen() override;
		virtual void SetClearColor(const Color& color_) override;
		virtual void SetViewportRect(const Rect& rect_) override;
		virtual void OnResize(int newWidth_, int newHeight_) override;

		virtual void SetWindingOrder(WindingOrder order_) override;
		virtual void SetCullFace(CullFace cullFace_) override;

		virtual Shader* GenerateAPIShader(StringID shaderResource_) override;
		virtual MaterialInfo* GenerateAPIMaterialInfo(const std::vector<Color>& colors_) override;
		virtual std::vector<MeshInfo*> GenerateAPIMeshInfos(const Mesh& mesh_) override;
		virtual MeshInfo* GenerateAPIDynamicMeshInfo(size_t numVertices_, size_t numIndices_) override;
		virtual TextureInfo* GenerateAPITextureInfo(const Texture& texture_) override;
		virtual FontInfo* GenerateAPIFontInfo(const FreetypeFont& font_) override;
		virtual MeshInstanceInfo* GenerateAPIMeshInstanceInfo(const Matrix4& transform_) override;

	private:
		DX12* dx12;
		DXR* dxr;

		DX12_RenderSurface* renderSurfacePtr;
	};
}

#endif //!GADGET_WIN32_DXR_RENDERER_H
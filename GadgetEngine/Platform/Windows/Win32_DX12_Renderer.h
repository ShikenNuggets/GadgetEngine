#ifndef GADGET_WIN32_DX12_RENDERER_H
#define GADGET_WIN32_DX12_RENDERER_H

#include "Graphics/Renderer.h"
#include "Graphics/DX12/DX12.h"

namespace Gadget{
	class Win32_DX12_Renderer : public Renderer{
	public:
		Win32_DX12_Renderer(int w_ = 800, int h_ = 600, int x_ = 0, int y_ = 0);
		virtual ~Win32_DX12_Renderer() override;

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
		virtual std::vector<MeshInfo*> GenerateAPIAnimMeshInfos(const AnimMesh& animMesh_) override;
		virtual TextureInfo* GenerateAPITextureInfo(const Texture& texture_) override;
		virtual FontInfo* GenerateAPIFontInfo(const FreetypeFont& font_) override;

	private:
		DX12_RenderSurface* renderSurfacePtr;
	};
}

#endif //!GADGET_WIN32_DX12_RENDERER_H
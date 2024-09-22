#ifndef GADGET_WIN32_RENDERER_H
#define GADGET_WIN32_RENDERER_H

//Hide warnings from external code that we can't/won't modify - WNF
#pragma warning(disable : 26819) //Unnanotated fallthrough in switch statement
#include <SDL.h>
#pragma warning(default : 26819)

#include "Animation/Components/AnimRenderComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/Mesh.h"
#include "Graphics/Texture.h"
#include "Graphics/Components/CameraComponent.h"
#include "Graphics/Components/DirectionalLightComponent.h"
#include "Graphics/Components/LightComponent.h"
#include "Graphics/Components/RenderComponent.h"
#include "Graphics/Components/SpotLightComponent.h"
#include "Graphics/GUI/GuiTextElement.h"
#include "Graphics/GUI/GuiTextureElement.h"
#include "Graphics/OpenGL/GL_ScreenQuad.h"
#include "Graphics/OpenGL/GL_Shader.h"
#include "Graphics/OpenGL/FrameBuffers/GL_DefaultFrameBuffer.h"

#undef APIENTRY //This just fixes a warning

namespace Gadget{
	//OpenGL Renderer for Windows
	class Win32_GL_Renderer : public Renderer{
	public:
		Win32_GL_Renderer(int w_ = 800, int h_ = 600, int x_ = 0, int y_ = 0);
		virtual ~Win32_GL_Renderer() override;

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

		static void __stdcall GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	protected:
		SDL_GLContext glContext;

		GL_DefaultFrameBuffer* mainFBO;
		GL_Shader* screenShader;
		GL_ScreenQuad* screenQuad;

	private:
		Array<CameraComponent*> camerasBuffer;
		Array<RenderComponent*> rendersBuffer;
		Array<AnimRenderComponent*> animRendersBuffer;
		Array<PointLightComponent*> pointLightsBuffer;
		Array<DirectionalLightComponent*> dirLightsBuffer;
		Array<SpotLightComponent*> spotLightsBuffer;

		Array<GuiTextElement*> guiTextsBuffer;
		Array<GuiTextureElement*> guiTexturesBuffer;
	};
}

#endif //!GADGET_WIN32_RENDERER_H
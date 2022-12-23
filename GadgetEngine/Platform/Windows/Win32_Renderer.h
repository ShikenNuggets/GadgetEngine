#ifndef GADGET_WIN32_RENDERER_H
#define GADGET_WIN32_RENDERER_H

#pragma warning(disable : 26819) //Kill unfixable warning from SDL2
#include <SDL.h>
#pragma warning(default : 26819)

#include "Graphics/Renderer.h"
#include "Graphics/Mesh.h"
#include "Graphics/Texture.h"
#include "Graphics/OpenGL/GL_ScreenQuad.h"
#include "Graphics/OpenGL/GL_Shader.h"
#include "Graphics/OpenGL/GL_CubemapInfo.h"
#include "Graphics/OpenGL/FrameBuffers/GL_DefaultFrameBuffer.h"

namespace Gadget{
	//OpenGL Renderer for Windows
	class Win32_Renderer : public Renderer{
	public:
		Win32_Renderer(int w_ = 800, int h_ = 600, int x_ = 0, int y_ = 0);
		virtual ~Win32_Renderer() override;

		virtual void PostInit() override;

		virtual void Render(const Scene* scene_) override;
		virtual void ClearScreen() override;
		virtual void SetClearColor(const Color& color_) override;
		virtual void SetViewportRect(const Rect& rect_) override;
		virtual void OnResize(int newWidth_, int newHeight_) override;

		virtual void SetWindingOrder(WindingOrder order_) override;
		virtual void SetCullFace(CullFace cullFace_) override;

		virtual Shader* GenerateAPIShader(StringID shaderResource_) override;
		virtual MeshInfo* GenerateAPIMeshInfo(const Mesh& mesh_) override;
		virtual TextureInfo* GenerateAPITextureInfo(const Texture& texture_) override;

		static void __stdcall GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	protected:
		SDL_GLContext glContext;

		GL_DefaultFrameBuffer* mainFBO;
		GL_Shader* screenShader;
		GL_ScreenQuad* screenQuad;

		//TODO - Model rendering, remove everything below Eventually(TM)
		Cubemap* cubemap;
		GL_CubemapInfo* cubemapInfo;
		Shader* skyboxShader;
	};
}

#endif //!GADGET_WIN32_RENDERER_H
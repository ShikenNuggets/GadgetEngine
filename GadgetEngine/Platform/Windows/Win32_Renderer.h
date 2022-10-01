#ifndef GADGET_WIN32_RENDERER_H
#define GADGET_WIN32_RENDERER_H

#pragma warning(disable : 26819) //Kill unfixable warning from SDL2
#include <SDL.h>
#pragma warning(default : 26819)

#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Graphics/LightSource.h"
#include "Graphics/Mesh.h"
#include "Graphics/Texture.h"
#include "Graphics/Materials/Material.h"
#include "Graphics/OpenGL/GL_FrameBuffer.h"
#include "Graphics/OpenGL/GL_MeshInfo.h"
#include "Graphics/OpenGL/GL_TextureInfo.h"
#include "Graphics/OpenGL/GL_ScreenQuad.h"
#include "Graphics/OpenGL/GL_Shader.h"

namespace Gadget{
	//OpenGL Renderer for Windows
	class Win32_Renderer : public Renderer{
	public:
		Win32_Renderer(int w_ = 800, int h_ = 600);
		virtual ~Win32_Renderer() override;

		virtual void PostInit() override;

		virtual void Render() override;
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

		GL_FrameBuffer* mainFBO;
		GL_Shader* screenShader;
		GL_ScreenQuad* screenQuad;

		//TODO - Model rendering, remove everything below Eventually(TM)
		Mesh* mesh;
		GL_MeshInfo* meshInfo;
		Material* material;
		Camera* camera;
		PointLight* light;
	};
}

#endif //!GADGET_WIN32_RENDERER_H
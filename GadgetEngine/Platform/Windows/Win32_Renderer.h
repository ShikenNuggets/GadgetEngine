#ifndef GADGET_WIN32_RENDERER_H
#define GADGET_WIN32_RENDERER_H

#include <SDL.h>

#include "Graphics/Renderer.h"

namespace Gadget{
	//OpenGL Renderer for Windows
	class Win32_Renderer : public Renderer{
	public:
		Win32_Renderer(int w_ = 800, int h_ = 600);
		virtual ~Win32_Renderer() override;

		virtual void Render() override;
		virtual void ClearScreen() override;
		virtual void SetClearColor(const Color& color_) override;
		virtual void SetWindingOrder(WindingOrder order_) override;
		virtual void SetCullFace(CullFace cullFace_) override;

	protected:
		SDL_GLContext glContext;
	};
}

#endif //!GADGET_WIN32_RENDERER_H
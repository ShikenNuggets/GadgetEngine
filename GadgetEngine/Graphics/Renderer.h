#ifndef GADGET_RENDERER_H
#define GADGET_RENDERER_H

#include <memory>

#include "Color.h"
#include "ViewportRect.h"
#include "Window.h"
#include "Math/MathObjects.h"

namespace Gadget{
	class Renderer{
	public:
		enum class WindingOrder{
			Clockwise,
			CounterClockwise
		};

		enum class CullFace{
			Back,
			Front,
			All
		};

		Renderer(WindingOrder order_ = WindingOrder::CounterClockwise, CullFace cullface_ = CullFace::Back) : window(nullptr), currentWindingOrder(order_), currentCullFace(cullface_){}
		virtual ~Renderer(){}

		std::weak_ptr<Window> GetWindow() const{ return window; }

		virtual void Render() = 0;
		virtual void ClearScreen() = 0;
		virtual void SetClearColor(const Color& color_) = 0;
		virtual void SetViewportRect(const Rect& rect_) = 0;

		virtual void SetWindingOrder(WindingOrder order_){ currentWindingOrder = order_; }
		virtual void SetCullFace(CullFace cullFace_){ currentCullFace = cullFace_; }

		float GetAspectRatio(){ return static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()); }
		void ResetViewportRect(){ SetViewportRect(ViewportRect::Fullscreen); }

	protected:
		std::shared_ptr<Window> window;
		WindingOrder currentWindingOrder;
		CullFace currentCullFace;
	};
}

#endif //!GADGET_RENDERER_H
#ifndef GADGET_RENDERER_H
#define GADGET_RENDERER_H

#include <memory>

#include "Color.h"
#include "Window.h"

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

		virtual void SetWindingOrder(WindingOrder order_){ currentWindingOrder = order_; }
		virtual void SetCullFace(CullFace cullFace_){ currentCullFace = cullFace_; }

	protected:
		std::shared_ptr<Window> window;
		WindingOrder currentWindingOrder;
		CullFace currentCullFace;
	};
}

#endif //!GADGET_RENDERER_H
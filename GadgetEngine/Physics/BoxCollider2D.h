#ifndef GADGET_BOX_COLLIDER_2D
#define GADGET_BOX_COLLIDER_2D

#include "Collider.h"

namespace Gadget{
	class BoxCollider2D : public Collider{
	public:
		BoxCollider2D(GameObject* parent_, float width_ = 1.0f, float height_ = 1.0f) : Collider(parent_), width(width_), height(height_){}

		float GetWidth() const{ return width; }
		float GetHeight() const{ return height; }

		void SetWidth(float width_){ width = width_; }
		void SetHeight(float height_){ height = height_; }

	private:
		float width;
		float height;
	};
}

#endif //!GADGET_BOX_COLLIDER_2D
#ifndef GADGET_BOX_COLLIDER_2D
#define GADGET_BOX_COLLIDER_2D

#include "Collider.h"

namespace Gadget{
	class BoxCollider2D : public Collider{
	public:
		BoxCollider2D(GameObject* parent_, float width_ = 1.0f, float height_ = 1.0f, bool isTrigger_ = false) : Collider(parent_, isTrigger_), width(width_), height(height_){}

		float GetWidth() const{ return width; }
		float GetHeight() const{ return height; }
		
		virtual Vector3 GetColliderSize() const override{
			return Vector3(
				parent->GetScale().x * (width / 2.0f),
				parent->GetScale().y * (height / 2.0f),
				parent->GetScale().z
			);
		}

		void SetWidth(float width_){ width = width_; }
		void SetHeight(float height_){ height = height_; }

	private:
		float width;
		float height;
	};
}

#endif //!GADGET_BOX_COLLIDER_2D
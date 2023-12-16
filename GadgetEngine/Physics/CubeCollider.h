#ifndef GADGET_CUBE_COLLIDER_H
#define GADGET_CUBE_COLLIDER_H

#include "Collider.h"

namespace Gadget{
	class CubeCollider : public Collider{
	public:
		CubeCollider(GameObject* parent_, float x_ = 1.0f, float y_ = 1.0f, float z_ = 1.0f, bool isTrigger_ = false) : Collider(SID("CubeCollider"), parent_, ColliderShape::Cube, isTrigger_), size(x_, y_, z_){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(x_));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(y_));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(z_));
		}

		CubeCollider(GUID parentGUID_, float x_ = 1.0f, float y_ = 1.0f, float z_ = 1.0f, bool isTrigger_ = false) : Collider(SID("CubeCollider"), parentGUID_, ColliderShape::Cube, isTrigger_), size(x_, y_, z_){
			GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(x_));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(y_));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(z_));
		}

		float GetX() const{ return size.x; }
		float GetY() const{ return size.y; }
		float GetZ() const{ return size.z; }

		virtual Vector3 GetColliderSize() const override{
			GADGET_BASIC_ASSERT(size.IsValid());
			return Vector3(
				(parent->GetScale().x * size.x) / 2.0f,
				(parent->GetScale().y * size.y) / 2.0f,
				(parent->GetScale().z * size.z) / 2.0f
			);
		}

		void SetX(float x_){ size.x = x_; }
		void SetY(float y_){ size.y = y_; }
		void SetZ(float z_){ size.z = z_; }

	private:
		Vector3 size;
	};
}

#endif //!GADGET_CUBE_COLLIDER_H
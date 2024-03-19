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

		CubeCollider(const ComponentProperties& props_) : Collider(props_, ColliderShape::Cube), size(){
			Deserialize(props_);
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

		virtual ComponentProperties Serialize() const override{
			ComponentProperties props = Collider::Serialize();
			props.variables.Add(SID("Size_X"), size.x);
			props.variables.Add(SID("Size_Y"), size.y);
			props.variables.Add(SID("Size_Z"), size.z);

			return props;
		}

	protected:
		virtual void Deserialize(const ComponentProperties& props_) override{
			size.x = props_.variables.GetValue(SID("Size_X"), 1.0f).ToNumber<float>();
			size.y = props_.variables.GetValue(SID("Size_Y"), 1.0f).ToNumber<float>();
			size.z = props_.variables.GetValue(SID("Size_Z"), 1.0f).ToNumber<float>();

			GADGET_BASIC_ASSERT(Math::IsValidNumber(size.x));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(size.y));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(size.z));
		}

	private:
		Vector3 size;
	};
}

#endif //!GADGET_CUBE_COLLIDER_H
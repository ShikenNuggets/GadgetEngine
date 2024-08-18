#ifndef GADGET_BOX_COLLIDER_2D
#define GADGET_BOX_COLLIDER_2D

#include "Collider.h"

namespace Gadget{
	class BoxCollider2D : public Collider{
	public:
		BoxCollider2D(GameObject* parent_, float width_ = 1.0f, float height_ = 1.0f, bool isTrigger_ = false) : Collider(SID("BoxCollider2D"), parent_, ColliderShape::Box2D, isTrigger_), width(width_), height(height_){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(width_));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(height_));
		}

		BoxCollider2D(GUID parentGUID_, float width_ = 1.0f, float height_ = 1.0f, bool isTrigger_ = false) : Collider(SID("BoxCollider2D"), parentGUID_, ColliderShape::Box2D, isTrigger_), width(width_), height(height_){
			GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(width_));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(height_));
		}

		BoxCollider2D(const ComponentProperties& props_) : Collider(props_, ColliderShape::Box2D){
			BoxCollider2D::Deserialize(props_);
		}

		float GetWidth() const{ return width; }
		float GetHeight() const{ return height; }

		virtual btCollisionShape* CreateCollisionShape() const override{
			return new btBoxShape(BulletHelper::ConvertVector3(GetColliderSize()));
		}
		
		virtual Vector3 GetColliderSize() const override{
			GADGET_BASIC_ASSERT(parent != nullptr);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(width));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(height));

			return Vector3(
				(parent->GetScale().x * width) / 2.0f,
				(parent->GetScale().y * height) / 2.0f,
				parent->GetScale().z / 2.0f
			);
		}

		void SetWidth(float width_){ width = width_; }
		void SetHeight(float height_){ height = height_; }

		virtual ComponentProperties Serialize() const override{
			ComponentProperties props = Collider::Serialize();
			props.variables.Add(SID("Width"), width);
			props.variables.Add(SID("Height"), height);

			return props;
		}

	protected:
		virtual void Deserialize(const ComponentProperties& props_) override{
			width = props_.variables.GetValue(SID("Width"), 1.0f).ToNumber<float>();
			height = props_.variables.GetValue(SID("Height"), 1.0f).ToNumber<float>();

			GADGET_BASIC_ASSERT(Math::IsValidNumber(width));
			GADGET_BASIC_ASSERT(Math::IsValidNumber(height));
		}

	private:
		float width;
		float height;
	};
}

#endif //!GADGET_BOX_COLLIDER_2D
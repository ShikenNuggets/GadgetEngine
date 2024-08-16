#ifndef GADGET_PHYSICS_SPHERE_COLLIDER_H
#define GADGET_PHYSICS_SPHERE_COLLIDER_H

#include "Physics/Collider.h"

namespace Gadget{
	class SphereCollider : public Collider{
	public:
		SphereCollider(GameObject* parent_, float radius_ = 1.0f, bool isTrigger_ = false) : Collider(SID("SphereCollider"), parent_, ColliderShape::Sphere, isTrigger_), radius(radius_){
			GADGET_BASIC_ASSERT(parent_ != nullptr && parent->GetGUID() != GUID::Invalid);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(radius_));
		}

		SphereCollider(GUID parentGUID_, float radius_ = 1.0f, bool isTrigger_ = false) : Collider(SID("SphereCollider"), parentGUID_, ColliderShape::Sphere, isTrigger_), radius(radius_){
			GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(radius_));
		}

		SphereCollider(const ComponentProperties& props_) : Collider(props_, ColliderShape::Sphere), radius(1.0f){
			SphereCollider::Deserialize(props_);
		}

		float GetRadius() const{ return radius; }

		virtual Vector3 GetColliderSize() const override{
			GADGET_BASIC_ASSERT(Math::IsValidNumber(radius));
			return Vector3(
				(parent->GetScale().x * radius) / 2.0f,
				(parent->GetScale().y * radius) / 2.0f,
				(parent->GetScale().z * radius) / 2.0f
			);
		}

		void SetRadius(float r_){ radius = r_; }

		virtual ComponentProperties Serialize() const override{
			ComponentProperties props = Collider::Serialize();
			props.variables.Add(SID("Radius"), radius);

			return props;
		}

	protected:
		virtual void Deserialize(const ComponentProperties& props_) override{
			radius = props_.variables.GetValue(SID("Radius"), 1.0f).ToNumber<float>();
			GADGET_BASIC_ASSERT(Math::IsValidNumber(radius));
		}

	private:
		float radius;
	};
}

#endif //!GADGET_PHYSICS_SPHERE_COLLIDER_H
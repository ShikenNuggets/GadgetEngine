#ifndef GADGET_COLLIDER_H
#define GADGET_COLLIDER_H

#include "Game/Component.h"
#include "Game/GameObject.h"
#include "Math/Vector.h"
#include "Physics/BulletHelper.h"

namespace Gadget{
	enum class ColliderShape{
		None = 0,
		Box2D,
		Circle2D,
		Cube,
		Sphere,

		ColliderShape_MAX //Do not put anything below this!
	};

	class Collider : public Component{
	public:
		Collider(StringID typeName_, GameObject* parent_, ColliderShape shape_, bool isTrigger_);
		Collider(StringID typeName_, GUID parentGUID_, ColliderShape shape_, bool isTrigger_);
		Collider(const ComponentProperties& props_, ColliderShape shape_);
		virtual ~Collider();

		static Collider* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static std::vector<Collider*> GetComponents(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.GetComponents(objectGuid_);
		}

		virtual void OnActivated() override;
		virtual void OnTransformModified() override;

		//Remove the collider from the physics sim and re-add it
		void Reset();

		ColliderShape GetShape() const{ return shape; }
		bool IsTrigger() const{ return isTrigger; }

		virtual Vector3 GetColliderSize() const{
			GADGET_BASIC_ASSERT(parent != nullptr);
			GADGET_BASIC_ASSERT(parent->GetScale().IsValid());
			return parent->GetScale();
		}

		virtual ComponentProperties Serialize() const override;

	protected:
		friend class PhysManager;
		friend struct BulletCollisionResultCallback;

		const ColliderShape shape;
		bool isTrigger;

		btRigidBody* bulletRb;

		virtual void Deserialize(const ComponentProperties& props_) override;

	private:
		static ComponentCollection<Collider> componentCollection;
	};
}

#endif //!GADGET_COLLIDER_H
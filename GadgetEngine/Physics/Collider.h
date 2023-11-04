#ifndef GADGET_COLLIDER_H
#define GADGET_COLLIDER_H

#include "BulletHelper.h"
#include "Game/Component.h"
#include "Game/GameObject.h"
#include "Math/Vector.h"

namespace Gadget{
	enum class ColliderShape{
		None = 0,
		Box2D,
		Circle2D,
		Cube,
		Sphere
	};

	class Collider : public Component{
	public:
		Collider(GameObject* parent_, ColliderShape shape_, bool isTrigger_);
		virtual ~Collider();

		Collider* Get(GUID objectGuid_){ return componentCollection.Get(objectGuid_); }

		virtual void OnActivated() override;
		virtual void OnTransformModified() override;

		//Remove the collider from the physics sim and re-add it
		void Reset();

		ColliderShape GetShape() const{ return shape; }
		bool IsTrigger() const{ return isTrigger; }

		virtual Vector3 GetColliderSize() const{ return parent->GetScale(); }

	protected:
		friend class PhysManager;
		friend struct BulletCollisionResultCallback;

		const ColliderShape shape;
		bool isTrigger;

		btRigidBody* bulletRb;

	private:
		static ComponentCollection<Collider> componentCollection;
	};
}

#endif //!GADGET_COLLIDER_H
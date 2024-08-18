#ifndef GADGET_PHYSICS_MESH_COLLIDER_H
#define GADGET_PHYSICS_MESH_COLLIDER_H

//Hide warnings from external code that we can't/won't modify - WNF
#pragma warning(disable : 26451)	//Possible arithmetic overflow
#pragma warning(disable : 26495)	//Uninitialized member variable
#pragma warning(disable : 4127)		//Conditional expression is constant
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#pragma warning(default : 26451)
#pragma warning(default : 26495)
#pragma warning(default : 4127)

#include "Data/Array.h"
#include "Physics/Collider.h"

namespace Gadget{
	class MeshCollider : public Collider{
	public:
		MeshCollider(GameObject* parent_, StringID modelName_, ColliderShape convexOrConcave_, bool isTrigger_ = false);
		MeshCollider(GUID parentGUID_, StringID modelName_, ColliderShape convexOrConcave_, bool isTrigger_ = false);
		MeshCollider(const ComponentProperties& props_);

		virtual btCollisionShape* CreateCollisionShape() const override;

		virtual Vector3 GetColliderSize() const override{
			GADGET_BASIC_ASSERT(size.IsValid());
			return Vector3(
				(parent->GetScale().x * size.x) / 2.0f,
				(parent->GetScale().y * size.y) / 2.0f,
				(parent->GetScale().z * size.z) / 2.0f
			);
		}

		void SetModel(StringID newModel_);

		virtual ComponentProperties Serialize() const override;

	protected:
		virtual void Deserialize(const ComponentProperties& props_) override;

	private:
		StringID modelName;
		Vector3 size;

		Array<btTriangleMesh*> triMeshes;

		void RecalculateCollider();
	};
}

#endif //!GADGET_PHYSICS_MESH_COLLIDER_H
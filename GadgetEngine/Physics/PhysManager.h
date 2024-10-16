#ifndef GADGET_PHYS_MANAGER_H
#define GADGET_PHYS_MANAGER_H

#include <vector>

#include "Physics/BulletHelper.h"
#include "Physics/Collider.h"
#include "Physics/Rigidbody.h"
#include "Game/Scene.h"

namespace Gadget{
	struct BulletCollisionResultCallback : public btCollisionWorld::ContactResultCallback{
		virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override;
	};

	class PhysManager{
	public:
		PhysManager();
		~PhysManager();

		void Update(Scene* scene_, float deltaTime_);

		void SetWorldGravity(float gravity_ = -9.81f);

		btRigidBody* AddToSimulation(const Collider* col_, const Rigidbody* rb_);
		void RemoveFromSimulation(btRigidBody* brb_);

		void ForceUpdateAABB(btRigidBody* brb_);

	private:
		static PhysManager* instance;

		friend struct BulletCollisionResultCallback;

		//Bullet physics objects
		btDiscreteDynamicsWorld* bulletDynamicsWorld;
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfig;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;

		std::vector<btCollisionShape*> cachedCollisionShapes;

		Array<Rigidbody*> rigidbodiesBuffer;
		Array<Collider*> collidersBuffer;

		void HandleCollisionResponse(Collider* collider_, Collider* other_);
	};
}

#endif //!GADGET_PHYS_MANAGER_H
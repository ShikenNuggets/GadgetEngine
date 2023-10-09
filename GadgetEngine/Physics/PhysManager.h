#ifndef GADGET_PHYS_MANAGER_H
#define GADGET_PHYS_MANAGER_H

#include "BulletHelper.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Game/Scene.h"

namespace Gadget{
	class PhysManager{
	public:
		PhysManager();
		~PhysManager();

		void Update(Scene* scene_, float deltaTime_);

		btRigidBody* AddToSimulation(const Collider* col_, const Rigidbody* rb_);
		void RemoveFromSimulation(btRigidBody* brb_);

	private:
		static PhysManager* instance;

		//Bullet physics objects
		btDiscreteDynamicsWorld* bulletDynamicsWorld;
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfig;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;

		std::vector<btCollisionShape*> cachedCollisionShapes;

		void HandleCollisionResponse(btBroadphasePair collisionPair_, Collider* collider_, Collider* other_);
		btCollisionShape* CreateCollisionShape(const Collider* col_);
	};
}

#endif //!GADGET_PHYS_MANAGER_H
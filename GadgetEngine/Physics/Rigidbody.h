#ifndef GADGET_RIGIDBODY_H
#define GADGET_RIGIDBODY_H

#include "BulletHelper.h"
#include "CollisionSystem.h"
#include "Game/Component.h"
#include "Math/Vector.h"

namespace Gadget{
	class Rigidbody : public Component{
	public:
		Rigidbody(GameObject* parent_, float mass_, bool useGravity_);
		~Rigidbody();

		void Update(float deltaTime_);

		//Apply a force in Newtons
		void AddForce(const Vector3& force_);
		void AddVelocity(const Vector3& vel_);
		void AddVelocity(float x_, float y_, float z_);

		float GetMass() const{ return mass; }
		Vector3 GetVelocity() const;
		bool UseGravity() const{ return useGravity; }

		void SetMass(float mass_){ mass = mass_; }
		void SetVelocity(const Vector3& velocity_);
		void SetVelocity(float x_, float y_, float z_);

		void CollisionResponse(const Collision& collision_);

	private:
		friend class Collider;

		float mass; //Mass in kg
		bool useGravity;

		btRigidBody* bulletRb;
	};
}

#endif //!GADGET_RIGIDBODY_H
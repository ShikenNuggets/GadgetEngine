#ifndef GADGET_RIGIDBODY_H
#define GADGET_RIGIDBODY_H

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

		float GetMass() const{ return mass; }
		Vector3 GetVelocity() const{ return velocity; }
		Vector3 GetAcceleration() const{ return acceleration; }
		bool UseGravity() const{ return useGravity; }

		void SetMass(float mass_){ mass = mass_; }
		void SetVelocity(const Vector3& velocity_){ velocity = velocity_; }
		void SetAcceleration(const Vector3& accel_){ acceleration = accel_; }

	private:
		float mass; //Mass in kg
		Vector3 velocity; //Velocity in m/s
		Vector3 acceleration;
		bool useGravity;
	};
}

#endif //!GADGET_RIGIDBODY_H
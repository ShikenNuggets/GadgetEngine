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
		void AddVelocity(const Vector3& vel_);
		void AddVelocity(float x_, float y_, float z_);

		float GetMass() const{ return mass; }
		Vector3 GetVelocity() const{ return velocity; }
		Vector3 GetAcceleration() const{ return acceleration; }
		bool UseGravity() const{ return useGravity; }

		void SetMass(float mass_){ mass = mass_; }
		void SetVelocity(const Vector3& velocity_){ velocity = velocity_; }
		void SetVelocity(float x_, float y_, float z_){ velocity = Vector3(x_, y_, z_); }
		void SetAcceleration(const Vector3& accel_){ acceleration = accel_; }
		void SetAcceleration(float x_, float y_, float z_){ acceleration = Vector3(x_, y_, z_); }

	private:
		float mass; //Mass in kg
		Vector3 velocity; //Velocity in m/s
		Vector3 acceleration;
		bool useGravity;
	};
}

#endif //!GADGET_RIGIDBODY_H
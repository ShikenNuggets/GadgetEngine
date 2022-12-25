#ifndef GADGET_RIGIDBODY_H
#define GADGET_RIGIDBODY_H

#include "Game/Component.h"
#include "Math/Vector.h"

namespace Gadget{
	class Rigidbody : public Component{
	public:
		Rigidbody(GameObject* parent_, bool useGravity_);
		~Rigidbody();

		void Update(float deltaTime_);

		Vector3 GetVelocity() const{ return velocity; }
		Vector3 GetAcceleration() const{ return acceleration; }
		bool UseGravity() const{ return true; }

		void SetVelocity(const Vector3& velocity_){ velocity = velocity_; }
		void SetAcceleration(const Vector3& accel_){ acceleration = accel_; }

	private:
		Vector3 velocity;
		Vector3 acceleration;
		bool useGravity;
	};
}

#endif //!GADGET_RIGIDBODY_H
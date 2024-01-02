#ifndef GADGET_RIGIDBODY_H
#define GADGET_RIGIDBODY_H

#include "BulletHelper.h"
#include "CollisionSystem.h"
#include "Game/Component.h"
#include "Math/Vector.h"

namespace Gadget{
	enum class FreezeRotationType{
		None,
		FreezeX,
		FreezeY,
		FreezeZ,
		FreezeXY,
		FreezeXZ,
		FreezeYZ,
		FreezeAll,

		FreezeRotationType_MAX //Do not put any values below this!
	};

	class Rigidbody : public Component{
	public:
		Rigidbody(GameObject* parent_, float mass_, bool useGravity_, FreezeRotationType freezeType_ = FreezeRotationType::None);
		Rigidbody(GUID parentGUID_, float mass_, bool useGravity_, FreezeRotationType freezeType_ = FreezeRotationType::None);
		Rigidbody(const ComponentProperties& props_);
		~Rigidbody();

		static Rigidbody* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static std::vector<Rigidbody*> GetComponents(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.GetComponents(objectGuid_);
		}

		void Update(float deltaTime_);

		//Apply a force in Newtons
		void AddForce(const Vector3& force_);
		void AddVelocity(const Vector3& vel_);
		void AddVelocity(float x_, float y_, float z_);

		float GetMass() const{ return mass; }
		Vector3 GetVelocity() const;
		bool UseGravity() const{ return useGravity; }
		FreezeRotationType GetFreezeRotation() const{ return freezeRotation; }

		void SetMass(float mass_){ mass = mass_; }
		void SetVelocity(const Vector3& velocity_);
		void SetVelocity(float x_, float y_, float z_);
		void FreezeRotation(FreezeRotationType type_);

		void ClearForces();

		void CollisionResponse(const Collision& collision_);

		//Remove the rigidbody from the physics sim and re-add it
		void Reset();

		virtual ComponentProperties Serialize() const override;

	protected:
		virtual void Deserialize(const ComponentProperties& props_) override;

	private:
		friend class Collider;

		float mass; //Mass in kg
		bool useGravity;
		FreezeRotationType freezeRotation;

		btRigidBody* bulletRb;

		static ComponentCollection<Rigidbody> componentCollection;
	};
}

#endif //!GADGET_RIGIDBODY_H
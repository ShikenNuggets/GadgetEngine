#include "Rigidbody.h"

#include "Game/GameObject.h"

using namespace Gadget;

ComponentCollection<Rigidbody> Rigidbody::componentCollection;

Rigidbody::Rigidbody(GameObject* parent_, float mass_, bool useGravity_, FreezeRotationType freezeType_) : Component(SID("Rigidbody"), parent_), mass(mass_), useGravity(useGravity_), bulletRb(nullptr), freezeRotation(freezeType_){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(mass_));
	GADGET_BASIC_ASSERT(!Math::IsNearZero(mass));
	GADGET_BASIC_ASSERT(mass > 0.0f);

	mass = Math::Clamp(0.0001f, Math::Infinity, mass); //0.0001 is completely arbitrary

	componentCollection.Add(this);
}

Rigidbody::Rigidbody(GUID parentGUID_, float mass_, bool useGravity_, FreezeRotationType freezeType_) : Component(SID("Rigidbody"), parentGUID_), mass(mass_), useGravity(useGravity_), bulletRb(nullptr), freezeRotation(freezeType_){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(mass_));
	GADGET_BASIC_ASSERT(!Math::IsNearZero(mass));
	GADGET_BASIC_ASSERT(mass > 0.0f);

	mass = Math::Clamp(0.0001f, Math::Infinity, mass); //0.0001 is completely arbitrary

	componentCollection.Add(this);
}

Rigidbody::~Rigidbody(){
	componentCollection.Remove(this);
}

void Rigidbody::Update([[maybe_unused]] float deltaTime_){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(deltaTime_));
	GADGET_BASIC_ASSERT(deltaTime_ >= 0.0f);

	if(parent == nullptr){
		Debug::Log("Update called on Rigidbody with no parent!", Debug::Error, __FILE__, __LINE__);
		return;
	}

	if(bulletRb == nullptr){
		return; //If we're not currently part of the physics sim, do nothing
	}

	btTransform bulletTransform;
	bulletRb->getMotionState()->getWorldTransform(bulletTransform);

	parent->SetPosition(BulletHelper::ConvertVector3(bulletTransform.getOrigin()));
	parent->SetRotation(BulletHelper::ConvertQuaternion(bulletTransform.getRotation()));
}

void Rigidbody::AddForce(const Vector3& force_){
	GADGET_BASIC_ASSERT(force_.IsValid());
	GADGET_BASIC_ASSERT(!Math::IsNearZero(mass));
	GADGET_BASIC_ASSERT(mass > 0.0f);
	GADGET_BASIC_ASSERT(bulletRb != nullptr);

	if(bulletRb == nullptr){
		//TODO - Apply force on next update
		return;
	}

	bulletRb->applyCentralForce(BulletHelper::ConvertVector3(force_));
}

void Rigidbody::AddVelocity(const Vector3& vel_){
	GADGET_BASIC_ASSERT(vel_.IsValid());
	bulletRb->setLinearVelocity(bulletRb->getLinearVelocity() + BulletHelper::ConvertVector3(vel_));
}

void Rigidbody::AddVelocity(float x_, float y_, float z_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(x_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(y_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(z_));

	AddVelocity(Vector3(x_, y_, z_));
}

Vector3 Rigidbody::GetVelocity() const{
	if(bulletRb == nullptr){
		return Vector3::Zero();
	}

	return BulletHelper::ConvertVector3(bulletRb->getLinearVelocity());
}

void Rigidbody::SetVelocity(const Vector3& vel_){
	GADGET_BASIC_ASSERT(vel_.IsValid());

	if(bulletRb == nullptr){
		//TODO - Set velocity on next update
		return;
	}

	if(!Math::IsNearZero(vel_.x) || !Math::IsNearZero(vel_.y) || !Math::IsNearZero(vel_.z)){
		bulletRb->activate(true); //Force activate the rigidbody if we're hard setting a non-zero velocity
	}

	bulletRb->setLinearVelocity(BulletHelper::ConvertVector3(vel_));
}

void Rigidbody::SetVelocity(float x_, float y_, float z_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(x_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(y_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(z_));

	SetVelocity(Vector3(x_, y_, z_));
}

void Rigidbody::FreezeRotation(FreezeRotationType type_){
	GADGET_BASIC_ASSERT(type_ < FreezeRotationType::FreezeRotationType_MAX);
	freezeRotation = type_;

	if(bulletRb != nullptr){
		switch(freezeRotation){
			case FreezeRotationType::None:
				bulletRb->setAngularFactor(btVector3(1.0f, 1.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeX:
				bulletRb->setAngularFactor(btVector3(0.0f, 1.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeY:
				bulletRb->setAngularFactor(btVector3(1.0f, 0.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeZ:
				bulletRb->setAngularFactor(btVector3(1.0f, 1.0f, 0.0f));
				break;
			case FreezeRotationType::FreezeXY:
				bulletRb->setAngularFactor(btVector3(0.0f, 0.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeXZ:
				bulletRb->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
				break;
			case FreezeRotationType::FreezeYZ:
				bulletRb->setAngularFactor(btVector3(1.0f, 0.0f, 0.0f));
				break;
			case FreezeRotationType::FreezeAll:
				bulletRb->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
				break;
			default:
				GADGET_ASSERT_NOT_IMPLEMENTED;
				break;
		}
	}
}

void Rigidbody::ClearForces(){
	if(bulletRb == nullptr){
		return;
	}

	bulletRb->clearForces();
}

void Rigidbody::CollisionResponse(const Collision& collision_){
	GADGET_BASIC_ASSERT(collision_.collisionVector.IsValid());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(collision_.overlapAmount));

	//TODO - This is obviously not proper collision response
	parent->Translate(-collision_.collisionVector.Normalized() * collision_.overlapAmount);
}

void Rigidbody::Reset(){
	Collider* collider = parent->GetComponent<Collider>();
	GADGET_BASIC_ASSERT(collider != nullptr); //All rigidbodies must have colliders

	if(collider != nullptr){
		collider->Reset();
	}
}
#include "Rigidbody.h"

#include "Game/GameObject.h"

using namespace Gadget;

Rigidbody::Rigidbody(GameObject* parent_, float mass_, bool useGravity_) : Component(parent_), mass(mass_), useGravity(useGravity_), bulletRb(nullptr){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(!Math::IsNearZero(mass));
	GADGET_BASIC_ASSERT(mass > 0.0f);

	mass = Math::Clamp(0.0001f, Math::Infinity, mass); //0.0001 is completely arbitrary
}

Rigidbody::~Rigidbody(){}

void Rigidbody::Update([[maybe_unused]] float deltaTime_){
	GADGET_BASIC_ASSERT(parent != nullptr);
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
	bulletRb->setLinearVelocity(bulletRb->getLinearVelocity() + BulletHelper::ConvertVector3(vel_));
}

void Rigidbody::AddVelocity(float x_, float y_, float z_){
	AddVelocity(Vector3(x_, y_, z_));
}

Vector3 Rigidbody::GetVelocity() const{
	if(bulletRb == nullptr){
		return Vector3::Zero();
	}

	return BulletHelper::ConvertVector3(bulletRb->getLinearVelocity());
}

void Rigidbody::SetVelocity(const Vector3& vel_){
	if(bulletRb == nullptr){
		//TODO - Set velocity on next update
		return;
	}

	bulletRb->setLinearVelocity(BulletHelper::ConvertVector3(vel_));
}

void Rigidbody::SetVelocity(float x_, float y_, float z_){
	SetVelocity(Vector3(x_, y_, z_));
}

void Rigidbody::CollisionResponse(const Collision& collision_){
	//TODO - This is obviously not proper collision response
	parent->Translate(-collision_.collisionVector.Normalized() * collision_.overlapAmount);
}
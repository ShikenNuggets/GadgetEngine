#include "Rigidbody.h"

#include "Game/GameObject.h"

using namespace Gadget;

Rigidbody::Rigidbody(GameObject* parent_, float mass_, bool useGravity_) : Component(parent_), mass(mass_), velocity(Vector3::Zero()), acceleration(Vector3::Zero()), useGravity(useGravity_){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(!Math::IsNearZero(mass));
	GADGET_BASIC_ASSERT(mass > 0.0f);

	mass = Math::Clamp(0.0001f, Math::Infinity, mass); //0.0001 is completely arbitrary
}

Rigidbody::~Rigidbody(){}

void Rigidbody::Update(float deltaTime_){
	GADGET_BASIC_ASSERT(parent != nullptr);
	if(parent == nullptr){
		Debug::Log("Update called on Rigidbody with no parent!", Debug::Error, __FILE__, __LINE__);
		return;
	}

	velocity += acceleration * deltaTime_;
	parent->Translate(velocity * deltaTime_);

	acceleration = Vector3::Zero();
}

void Rigidbody::AddForce(const Vector3& force_){
	GADGET_BASIC_ASSERT(!Math::IsNearZero(mass));
	GADGET_BASIC_ASSERT(mass > 0.0f);

	acceleration += force_ / mass;
}

void Rigidbody::AddVelocity(const Vector3& vel_){
	velocity += vel_;
}

void Rigidbody::AddVelocity(float x_, float y_, float z_){
	AddVelocity(Vector3(x_, y_, z_));
}

void Rigidbody::CollisionResponse(const Collision& collision_){
	//TODO - This is obviously not proper collision response
	parent->Translate(-collision_.collisionVector.Normalized() * collision_.overlapAmount);
}
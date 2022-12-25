#include "Rigidbody.h"

#include "Game/GameObject.h"

using namespace Gadget;

Rigidbody::Rigidbody(GameObject* parent_, bool useGravity_) : Component(parent_), velocity(Vector3::Zero()), acceleration(Vector3::Zero()), useGravity(useGravity_){}

Rigidbody::~Rigidbody(){}

void Rigidbody::Update(float deltaTime_){
	GADGET_BASIC_ASSERT(parent != nullptr);
	parent->Translate(velocity * deltaTime_);
}
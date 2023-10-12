#include "Collider.h"

#include "App.h"

using namespace Gadget;

Collider::~Collider(){
	if(bulletRb != nullptr){
		App::GetPhysics().RemoveFromSimulation(bulletRb);
	}
}

void Collider::OnActivated(){
	Reset();
	Component::OnActivated();
}

void Collider::OnTransformModified(){
	if(bulletRb == nullptr){
		return;
	}

	btTransform newTransform = BulletHelper::ConvertTransform(parent->GetTransform());
	if(bulletRb->getWorldTransform() == newTransform){
		return;
	}

	//If we modify the transform directly, we need to make sure those changes are reflected within the physics engine
	bulletRb->setWorldTransform(BulletHelper::ConvertTransform(parent->GetTransform()));
	bulletRb->getMotionState()->setWorldTransform(BulletHelper::ConvertTransform(parent->GetTransform()));
}

void Collider::Reset(){
	if(bulletRb != nullptr){
		App::GetPhysics().RemoveFromSimulation(bulletRb);
	}

	Rigidbody* rb = parent->GetComponent<Rigidbody>();
	bulletRb = App::GetPhysics().AddToSimulation(this, rb);

	if(rb != nullptr){
		rb->bulletRb = bulletRb;
	}

	bulletRb->setUserPointer(this);
}
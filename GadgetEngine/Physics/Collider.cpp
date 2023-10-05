#include "Collider.h"

#include "App.h"

using namespace Gadget;

Collider::~Collider(){
	if(bulletRb != nullptr){
		App::GetPhysics().RemoveFromSimulation(bulletRb);
	}
}

void Collider::OnActivated(){
	if(bulletRb != nullptr){
		App::GetPhysics().RemoveFromSimulation(bulletRb);
	}

	Rigidbody* rb = parent->GetComponent<Rigidbody>();
	bulletRb = App::GetPhysics().AddToSimulation(this, rb);

	if(rb != nullptr){
		rb->bulletRb = bulletRb;
	}

	Component::OnActivated();
}

void Collider::OnTransformModified(){
	if(bulletRb == nullptr){
		return;
	}

	//If we modify the transform directed, we need to make sure those changes are reflected within the physics engine
	bulletRb->getMotionState()->setWorldTransform(BulletHelper::ConvertTransform(parent->GetTransform()));
}
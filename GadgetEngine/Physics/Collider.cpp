#include "Collider.h"

#include "App.h"

using namespace Gadget;

ComponentCollection<Collider> Collider::componentCollection;

Collider::Collider(GameObject* parent_, ColliderShape shape_, bool isTrigger_) : Component(parent_), shape(shape_), isTrigger(isTrigger_), bulletRb(nullptr){
	GADGET_BASIC_ASSERT(parent_ != nullptr);
	GADGET_BASIC_ASSERT(shape_ < ColliderShape::ColliderShape_MAX);

	componentCollection.Add(this);
}

Collider::~Collider(){
	if(bulletRb != nullptr){
		App::GetPhysics().RemoveFromSimulation(bulletRb);
	}

	componentCollection.Remove(this);
}

void Collider::OnActivated(){
	Reset();
	Component::OnActivated();

	GADGET_BASIC_ASSERT(bulletRb != nullptr);
}

void Collider::OnTransformModified(){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(parent->GetTransform().position.IsValid());
	GADGET_BASIC_ASSERT(parent->GetTransform().rotation.IsValid());

	if(bulletRb == nullptr){
		return;
	}

	btTransform newTransform = BulletHelper::ConvertTransform(parent->GetTransform());
	if(bulletRb->getWorldTransform() == newTransform){
		return;
	}

	bulletRb->activate(true); //Force activate the rigidbody if we're hard setting a new position

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
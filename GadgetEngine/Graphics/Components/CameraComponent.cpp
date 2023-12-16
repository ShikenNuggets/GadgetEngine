#include "CameraComponent.h"

#include "App.h"

using namespace Gadget;

ComponentCollection<CameraComponent> CameraComponent::componentCollection = ComponentCollection<CameraComponent>();

CameraComponent::CameraComponent(GameObject* parent_, Camera::Projection projection_, const Rect& viewRect_) : Component(SID("CameraComponent"), parent_){
	GADGET_BASIC_ASSERT(parent_ != nullptr);
	GADGET_BASIC_ASSERT(parent_->GetGUID() != GUID::Invalid);
	GADGET_BASIC_ASSERT(projection_ < Camera::Projection::Projection_MAX);
	GADGET_BASIC_ASSERT(viewRect_.IsValid());

	camera = Camera(parent->GetPosition(), parent->GetRotation(), projection_, viewRect_);
	lastPosition = parent->GetPosition();
	lastRotation = parent->GetRotation();
	lastAspect = App::GetAspectRatio();

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(lastPosition.IsValid());
	GADGET_BASIC_ASSERT(lastRotation.IsValid());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(lastAspect));
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

CameraComponent::CameraComponent(GUID parentGUID_, Camera::Projection projection_, const Rect& viewRect_) : Component(SID("CameraComponent"), parentGUID_){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(projection_ < Camera::Projection::Projection_MAX);
	GADGET_BASIC_ASSERT(viewRect_.IsValid());

	camera = Camera(parent->GetPosition(), parent->GetRotation(), projection_, viewRect_);
	lastPosition = parent->GetPosition();
	lastRotation = parent->GetRotation();
	lastAspect = App::GetAspectRatio();

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(lastPosition.IsValid());
	GADGET_BASIC_ASSERT(lastRotation.IsValid());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(lastAspect));
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

CameraComponent::~CameraComponent(){
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);

	componentCollection.Remove(this);

	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == nullptr);
}

Matrix4 CameraComponent::GetUpdatedViewMatrix(){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(parent->GetPosition().IsValid());
	GADGET_BASIC_ASSERT(parent->GetRotation().IsValid());
	GADGET_BASIC_ASSERT(lastPosition.IsValid());
	GADGET_BASIC_ASSERT(lastRotation.IsValid());

	if(!lastPosition.IsNear(parent->GetPosition()) || !lastRotation.IsNear(parent->GetRotation())){
		camera.CalculateViewMatrix(parent->GetPosition(), parent->GetRotation());
		lastPosition = parent->GetPosition();
		lastRotation = parent->GetRotation();
	}

	GADGET_BASIC_ASSERT(lastPosition.IsValid());
	GADGET_BASIC_ASSERT(lastRotation.IsValid());
	GADGET_BASIC_ASSERT(camera.GetViewMatrix().IsValid());

	return camera.GetViewMatrix();
}

Matrix4 CameraComponent::GetUpdatedProjectionMatrix(){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(App::GetAspectRatio()));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(lastAspect));

	if(!Math::Near(lastAspect, App::GetAspectRatio())){
		camera.SetAspect(App::GetAspectRatio()); //This calls CalculateProjectionMatrix() on its own
		lastAspect = App::GetAspectRatio();
	}

	GADGET_BASIC_ASSERT(Math::IsValidNumber(lastAspect));
	GADGET_BASIC_ASSERT(camera.GetProjectionMatrix().IsValid());

	return camera.GetProjectionMatrix();
}
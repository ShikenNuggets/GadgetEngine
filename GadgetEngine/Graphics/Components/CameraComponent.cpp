#include "CameraComponent.h"

#include "App.h"

using namespace Gadget;

CameraComponent::CameraComponent(GameObject* parent_, Camera::Projection projection_, const Rect& viewRect_) : Component(parent_){
	camera = Camera(parent->GetPosition(), parent->GetRotation(), projection_, viewRect_);
	lastPosition = parent->GetPosition();
	lastRotation = parent->GetRotation();
	lastAspect = App::GetInstance()->GetAspectRatio();
}

CameraComponent::~CameraComponent(){}

Matrix4 CameraComponent::GetUpdatedViewMatrix(){
	if(!lastPosition.IsNear(parent->GetPosition()) || !lastRotation.IsNear(parent->GetRotation())){
		camera.CalculateViewMatrix(parent->GetPosition(), parent->GetRotation());
		lastPosition = parent->GetPosition();
		lastRotation = parent->GetRotation();
	}

	return camera.GetViewMatrix();
}

Matrix4 CameraComponent::GetUpdatedProjectionMatrix(){
	if(!Math::Near(lastAspect, App::GetInstance()->GetAspectRatio())){
		camera.SetAspect(App::GetInstance()->GetAspectRatio()); //This calls CalculateProjectionMatrix() on its own
		lastAspect = App::GetInstance()->GetAspectRatio();
	}

	return camera.GetProjectionMatrix();
}
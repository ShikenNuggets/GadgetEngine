#include "CameraComponent.h"

#include "Game/GameObject.h"

using namespace Gadget;

CameraComponent::CameraComponent(GameObject* parent_, Camera::Projection projection_, const Rect& viewRect_) : Component(parent_){
	camera = Camera(parent->GetPosition(), parent->GetRotation(), projection_, viewRect_);
	lastPosition = parent->GetPosition();
	lastRotation = parent->GetRotation();
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
#include "CameraComponent.h"

#include "Game/GameObject.h"

using namespace Gadget;

CameraComponent::CameraComponent(GameObject* parent_, Camera::Projection projection_, const Rect& viewRect_) : Component(parent_){
	camera = Camera(parent->GetPosition(), parent->GetRotation(), projection_, viewRect_);
}

CameraComponent::~CameraComponent(){}
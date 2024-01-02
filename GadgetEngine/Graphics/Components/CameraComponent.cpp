#include "CameraComponent.h"

#include "App.h"

using namespace Gadget;

const StringID CameraComponent::type = SID("CameraComponent");
ComponentCollection<CameraComponent> CameraComponent::componentCollection = ComponentCollection<CameraComponent>();

CameraComponent::CameraComponent(GameObject* parent_, Camera::Projection projection_, const Rect& viewRect_) : Component(type, parent_){
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

CameraComponent::CameraComponent(GUID parentGUID_, Camera::Projection projection_, const Rect& viewRect_) : Component(type, parentGUID_){
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

CameraComponent::CameraComponent(const ComponentProperties& props_) : Component(props_){
	GADGET_BASIC_ASSERT(props_.typeName == CameraComponent::type);

	Deserialize(props_);

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

ComponentProperties CameraComponent::Serialize() const{
	ComponentProperties props = Component::Serialize();
	props.variables.Add(SID("Projection"), (int)camera.GetCurrentProjection());
	props.variables.Add(SID("ViewRect_X"), camera.GetViewportRect().x);
	props.variables.Add(SID("ViewRect_Y"), camera.GetViewportRect().y);
	props.variables.Add(SID("ViewRect_W"), camera.GetViewportRect().w);
	props.variables.Add(SID("ViewRect_H"), camera.GetViewportRect().h);
}

void CameraComponent::Deserialize(const ComponentProperties& props_){
	Camera::Projection proj = (Camera::Projection)props_.variables.GetValue(SID("Projection")).ToNumber<int>();
	GADGET_BASIC_ASSERT((int)proj > 0);
	GADGET_BASIC_ASSERT(proj < Camera::Projection::Projection_MAX);
	if((int)proj < 0 || proj >= Camera::Projection::Projection_MAX){
		proj = Camera::Projection::Perspective; //Default to perspective if something went wrong
	}

	Rect viewRect = Rect();
	viewRect.x = props_.variables.GetValue(SID("ViewRectX")).ToNumber<float>();
	viewRect.y = props_.variables.GetValue(SID("ViewRectY")).ToNumber<float>();
	viewRect.w = props_.variables.GetValue(SID("ViewRectW")).ToNumber<float>();
	viewRect.h = props_.variables.GetValue(SID("ViewRectH")).ToNumber<float>();
	GADGET_BASIC_ASSERT(viewRect.IsValid());
	if(!viewRect.IsValid()){
		viewRect = ViewportRect::Fullscreen;
	}

	camera = Camera(parent->GetPosition(), parent->GetRotation(), proj, viewRect);
}
#include "CanvasSceneComponent.h"

using namespace Gadget;

CanvasSceneComponent::CanvasSceneComponent(Scene* parent_, GuiCanvas* canvas_) : SceneComponent(parent_), canvas(canvas_){
	GADGET_BASIC_ASSERT(parent_ != nullptr);
	GADGET_BASIC_ASSERT(canvas != nullptr);
}

CanvasSceneComponent::~CanvasSceneComponent(){
	delete canvas;
}

void CanvasSceneComponent::OnUpdate(float deltaTime_){
	GADGET_BASIC_ASSERT(canvas != nullptr);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(deltaTime_) && deltaTime_ >= 0.0f);

	canvas->Update(deltaTime_);
}
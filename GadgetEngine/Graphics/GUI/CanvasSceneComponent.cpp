#include "CanvasSceneComponent.h"

using namespace Gadget;

CanvasSceneComponent::CanvasSceneComponent(Scene* parent_, GuiCanvas* canvas_) : SceneComponent(parent_), canvas(canvas_){
	GADGET_BASIC_ASSERT(canvas != nullptr);
}

CanvasSceneComponent::~CanvasSceneComponent(){
	delete canvas;
}

void CanvasSceneComponent::OnUpdate(float deltaTime_){
	GADGET_BASIC_ASSERT(canvas != nullptr);
	canvas->Update(deltaTime_);
}
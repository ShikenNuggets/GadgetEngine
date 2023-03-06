#include "CanvasSceneComponent.h"

using namespace Gadget;

CanvasSceneComponent::CanvasSceneComponent(Scene* parent_, const GuiCanvas& canvas_) : SceneComponent(parent_), canvas(canvas_){}

void CanvasSceneComponent::OnUpdate(float deltaTime_){
	canvas.Update(deltaTime_);
}
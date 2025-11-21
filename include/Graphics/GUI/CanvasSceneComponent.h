#ifndef GADGET_CANVAS_SCENE_COMPONENT_H
#define GADGET_CANVAS_SCENE_COMPONENT_H

#include "Game/SceneComponent.h"
#include "Graphics/GUI/GuiCanvas.h"

namespace Gadget{
	class CanvasSceneComponent : public SceneComponent{
	public:
		CanvasSceneComponent(Scene* parent_, GuiCanvas* canvas_);
		~CanvasSceneComponent();

		virtual void OnUpdate(float deltaTime_) override;

		GuiCanvas& GetCanvas(){ return *canvas; }
		const GuiCanvas& GetCanvas() const{ return *canvas; }

	private:
		GuiCanvas* canvas;
	};
}

#endif //!GADGET_CANVAS_SCENE_COMPONENT_H
#ifndef DEMO_REEL_SCENE_H
#define DEMO_REEL_SCENE_H

#include <Game/Scene.h>

#include <Graphics/Components/CameraComponent.h>
#include <Graphics/Components/RenderComponent.h>
#include <Graphics/GUI/CanvasSceneComponent.h>
#include <Graphics/GUI/FpsDisplayElement.h>
#include <Graphics/GUI/GuiButton.h>
#include <Graphics/GUI/GuiTextElement.h>
#include <Graphics/GUI/GuiTextureElement.h>
#include <Physics/BoxCollider2D.h>
#include <Physics/Rigidbody.h>

namespace DemoReel{
	class DemoReelScene : public Gadget::Scene{
	public:
		DemoReelScene() : Gadget::Scene(SID("DemoReelScene")){}

	protected:
		virtual void SetToDefaultState() override{
			Gadget::Scene::SetToDefaultState();
		}
	};
}
#endif //!DEMO_REEL_SCENE_H
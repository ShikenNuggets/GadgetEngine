#ifndef PONG_MENU_SCENE_H
#define PONG_MENU_SCENE_H

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

#include "PongMenuSceneHandler.h"

namespace Pong{
	class PongMenuScene : public Gadget::Scene{
	public:
		PongMenuScene() : Gadget::Scene(SID("PongMenuScene")){}

	protected:
		virtual void SetToDefaultState() final override{
			Gadget::Scene::SetToDefaultState();

			PongMenuSceneHandler* sceneHandler = new PongMenuSceneHandler(this);
			AddSceneComponent(sceneHandler);

			Gadget::GuiButton* soloButton = new Gadget::GuiButton(SID("PlayEasyButton"), "Solo (Easy)", SID("ArialFont"), SID("CubeTexture"), Gadget::Vector2(0.0f, 0.5f), Gadget::Vector2(0.5f, 0.2f), Gadget::GuiAnchor::Center);
			Gadget::GuiButton* soloHardButton = new Gadget::GuiButton(SID("PlayHardButton"), "Solo (Hard)", SID("ArialFont"), SID("CubeTexture"), Gadget::Vector2(0.0f, 0.0f), Gadget::Vector2(0.5f, 0.2f), Gadget::GuiAnchor::Center);
			Gadget::GuiButton* multiButton = new Gadget::GuiButton(SID("MultiButton"), "2 Player", SID("ArialFont"), SID("CubeTexture"), Gadget::Vector2(0.0f, -0.5f), Gadget::Vector2(0.5f, 0.2f), Gadget::GuiAnchor::Center);

			soloButton->SetOnClickCallback(std::bind(&PongMenuSceneHandler::LoadOnePlayerGame, sceneHandler));
			soloHardButton->SetOnClickCallback(std::bind(&PongMenuSceneHandler::LoadOnePlayerHardGame, sceneHandler));
			multiButton->SetOnClickCallback(std::bind(&PongMenuSceneHandler::LoadTwoPlayerGame, sceneHandler));

			Gadget::GuiCanvas* gc = new Gadget::GuiCanvas(SID("MainCanvas"));

			gc->AddElement(soloButton);
			gc->AddElement(soloHardButton);
			gc->AddElement(multiButton);
			gc->AddElement(new Gadget::FpsDisplayElement(SID("FPS"), SID("ArialFont"), Gadget::Vector2(1.65f, -0.95f), Gadget::Vector2(0.125f, 0.125f), Gadget::GuiAnchor::Center));
			AddSceneComponent(new Gadget::CanvasSceneComponent(this, gc));

			auto camera = new Gadget::GameObject();
			camera->SetPosition(0.0f, 0.0f, 0.0f);
			camera->AddComponent(new Gadget::CameraComponent(camera, Gadget::Camera::Projection::Orthographic));
			CreateObject(camera);
		}
	};
}
#endif //!PONG_MENU_SCENE_H
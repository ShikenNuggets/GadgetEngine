#ifndef GADGET_EXAMPLE_SCENE_H
#define GADGET_EXAMPLE_SCENE_H

#include <Animation/Components/AnimRenderComponent.h>
#include <Game/Scene.h>
#include <Graphics/Components/CameraComponent.h>
#include <Graphics/Components/LightComponent.h>
#include <Graphics/Components/RenderComponent.h>
#include <Graphics/Components/SkyboxComponent.h>
#include <Graphics/GUI/FpsDisplayElement.h>
#include <Graphics/Materials/EngineMaterial.h>
#include <Physics/CubeCollider.h>
#include <Physics/Rigidbody.h>

#include "CameraController.h"
#include "ObjectRotator.h"

namespace Example{
	class ExampleScene : public Gadget::Scene{
	public:
		ExampleScene() : Gadget::Scene(SID("ExampleScene")){}

		virtual void Update(float deltaTime_) override{
			Scene::Update(deltaTime_);

			static bool normalTimeScale = true;
			if(Gadget::App::GetInput().GetButtonDown(Gadget::ButtonID::Keyboard_Q)){
				normalTimeScale = !normalTimeScale;

				if(!normalTimeScale){
					Gadget::App::GetTime().SetTimeScale(0.25f);
				}else{
					Gadget::App::GetTime().SetTimeScale(1.0f);
				}
			}
		}

	protected:
		virtual void SetToDefaultState() final override{
			Gadget::Scene::SetToDefaultState();

			AddSceneComponent(new Gadget::SkyboxComponent(this, SID("Skybox")));

			Gadget::GuiCanvas* gc = new Gadget::GuiCanvas(SID("MainCanvas"));
			gc->AddElement(new Gadget::FpsDisplayElement(SID("FPS"), SID("ArialFont"), Gadget::Vector2(1.65f, -0.95f), Gadget::Vector2(0.125f, 0.125f), Gadget::GuiAnchor::Center));
			AddSceneComponent(new Gadget::CanvasSceneComponent(this, gc));

			Gadget::EngineMaterial* redMaterial = new Gadget::EngineMaterial(Gadget::Color::Red());
			Gadget::EngineMaterial* blueMaterial = new Gadget::EngineMaterial(Gadget::Color::Blue());

			//Gadget::GameObject* cube = new Gadget::GameObject();
			//cube->SetPosition(0.0f, 2.5f, 0.0f);
			//cube->AddComponent(new Gadget::RenderComponent(cube->GetGUID(), SID("CubeModel"), SID("CubeTexture"), SID("DefaultShader")));
			//cube->AddComponent(new Gadget::RenderComponent(cube->GetGUID(), SID("CubeModel"), redMaterial));
			//cube->AddComponent(new Gadget::Rigidbody(cube, 100.0f, true));
			//cube->AddComponent(new Gadget::CubeCollider(cube));
			//cube->AddComponent(new ObjectRotator(cube));
			//CreateObject(cube);

			std::vector<Gadget::StringID> materials;
			materials.push_back(SID("BlueMaterial"));
			materials.push_back(SID("BlackMaterial"));

			Gadget::GameObject* player = new Gadget::GameObject(SID("Player"));
			player->SetScale(0.02f);
			auto* animRender = new Gadget::AnimRenderComponent(player, SID("YBotAnimModel"), materials);
			animRender->AddClip(SID("IdleAnim"));
			animRender->AddClip(SID("StandUpAnim"));
			animRender->AddClip(SID("CrouchWalkAnim"));
			player->AddComponent(animRender);
			CreateObject(player);

			Gadget::GameObject* floor = new Gadget::GameObject();
			floor->SetScale(Gadget::Vector3(5.0f, 0.01f, 5.0f));
			floor->AddComponent(new Gadget::RenderComponent(floor->GetGUID(), SID("CubeModel"), SID("BrickMaterial")));
			floor->AddComponent(new Gadget::CubeCollider(floor));
			CreateObject(floor);

			Gadget::GameObject* light = new Gadget::GameObject();
			light->SetPosition(2.0f, -1.0f, 1.0f);
			light->AddComponent(new Gadget::PointLightComponent(light));
			CreateObject(light);

			Gadget::GameObject* dirLight = new Gadget::GameObject(SID("DirLight"));
			dirLight->Rotate(-75.0f, 15.0f, 0.0f);
			dirLight->AddComponent(new Gadget::DirectionalLightComponent(dirLight));
			CreateObject(dirLight);

			Gadget::GameObject* camera = new Gadget::GameObject();
			camera->SetPosition(0.0f, 2.5f, 8.0f);
			camera->AddComponent(new Gadget::CameraComponent(camera));
			camera->AddComponent(new CameraController(camera));
			CreateObject(camera);
		}
	};
}

#endif //!GADGET_EXAMPLE_SCENE_H
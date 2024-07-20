#ifndef STAR_HAWX_GAMEPLAY_TEST_SCENE_H
#define STAR_HAWX_GAMEPLAY_TEST_SCENE_H

#include <Game/Scene.h>
#include <Graphics/Components/CameraComponent.h>
#include <Graphics/Components/SkyboxComponent.h>

#include "PlayerController.h"

namespace StarHawx{
	class GameplayTestScene : public Gadget::Scene{
	public:
		GameplayTestScene() : Gadget::Scene(SID("GameplayTest")){}

		virtual void SetToDefaultState() final override{
			Gadget::Scene::SetToDefaultState();

			//AddSceneComponent(new Gadget::SkyboxComponent(this, SID("Skybox")));

			Gadget::GameObject* player = new Gadget::GameObject();
			player->SetPosition(0.0f, 0.0f, 0.0f);
			player->AddComponent(new Gadget::RenderComponent(player->GetGUID(), SID("ArwingModel"), SID("BrickMaterial")));
			//cube->AddComponent(new Gadget::RenderComponent(cube->GetGUID(), SID("CubeModel"), redMaterial));
			player->AddComponent(new Gadget::Rigidbody(player, 100.0f, false));
			player->AddComponent(new Gadget::CubeCollider(player));
			player->AddComponent(new PlayerController(player));
			CreateObject(player);

			Gadget::GameObject* floor = new Gadget::GameObject();
			floor->SetPosition(Gadget::Vector3(0.0f, -2.5f, 0.0f));
			floor->SetScale(Gadget::Vector3(25.0f, 0.01f, 25.0f));
			floor->AddComponent(new Gadget::RenderComponent(floor->GetGUID(), SID("CubeModel"), SID("BrickMaterial")));
			//floor->AddComponent(new Gadget::RenderComponent(floor->GetGUID(), SID("CubeModel"), blueMaterial));
			floor->AddComponent(new Gadget::CubeCollider(floor));
			CreateObject(floor);

			Gadget::GameObject* light = new Gadget::GameObject();
			light->SetPosition(2.0f, -1.0f, 1.0f);
			light->AddComponent(new Gadget::PointLightComponent(light));
			CreateObject(light);

			Gadget::GameObject* camera = new Gadget::GameObject();
			camera->SetPosition(0.0f, 0.0f, 8.0f);
			camera->AddComponent(new Gadget::CameraComponent(camera));
			CreateObject(camera);
		}
	};
}

#endif //!STAR_HAWX_GAMEPLAY_TEST_SCENE_H
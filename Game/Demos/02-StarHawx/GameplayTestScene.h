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

			AddSceneComponent(new Gadget::SkyboxComponent(this, SID("Skybox")));

			std::vector<Gadget::StringID> materialNames;
			materialNames.push_back(SID("ArwingMaterial15"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial6"));
			materialNames.push_back(SID("ArwingMaterial7"));
			materialNames.push_back(SID("ArwingMaterial6"));
			materialNames.push_back(SID("ArwingMaterial3"));
			materialNames.push_back(SID("ArwingMaterial13"));
			materialNames.push_back(SID("ArwingMaterial10"));
			materialNames.push_back(SID("ArwingMaterial2"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial1"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial12"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial12"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial12"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial7"));
			materialNames.push_back(SID("ArwingMaterial3"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial1"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial13"));
			materialNames.push_back(SID("ArwingMaterial3"));
			materialNames.push_back(SID("ArwingMaterial5"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial5"));
			materialNames.push_back(SID("ArwingMaterial15"));
			materialNames.push_back(SID("ArwingMaterial10"));
			materialNames.push_back(SID("ArwingMaterial6"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial12"));
			materialNames.push_back(SID("ArwingMaterial6"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial14"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial8"));

			Gadget::GameObject* player = new Gadget::GameObject();
			player->SetPosition(0.0f, 0.0f, 0.0f);
			player->AddComponent(new Gadget::RenderComponent(player->GetGUID(), SID("ArwingModel"), materialNames));
			player->AddComponent(new Gadget::Rigidbody(player, 100.0f, false));
			player->AddComponent(new Gadget::CubeCollider(player));
			player->AddComponent(new PlayerController(player));
			CreateObject(player);

			Gadget::GameObject* floor = new Gadget::GameObject();
			floor->SetPosition(Gadget::Vector3(0.0f, -2.5f, 0.0f));
			floor->SetScale(Gadget::Vector3(25.0f, 0.01f, 25.0f));
			floor->AddComponent(new Gadget::RenderComponent(floor->GetGUID(), SID("CubeModel"), SID("BrickMaterial")));
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
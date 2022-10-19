#ifndef EXAMPLE_GAME_H
#define EXAMPLE_GAME_H

#include <App.h>
#include <GameInterface.h>
#include <Game/GameObject.h>
#include <Game/Scene.h>
#include <Graphics/Components/CameraComponent.h>
#include <Graphics/Components/LightComponent.h>
#include <Graphics/Components/RenderComponent.h>

namespace Example{
	class ExampleGame : public Gadget::GameInterface{
	public:
		ExampleGame() : GameInterface("Example Game"){}

		virtual void LoadGame() override{
			Gadget::GameObject* cube = new Gadget::GameObject();
			cube->AddComponent(new Gadget::RenderComponent(cube, SID("CubeModel"), SID("CubeTexture"), SID("DefaultShader")));

			Gadget::GameObject* light = new Gadget::GameObject();
			light->SetPosition(2.0f, 1.0f, 1.0f);
			light->AddComponent(new Gadget::PointLightComponent(light));

			Gadget::GameObject* camera = new Gadget::GameObject();
			camera->SetPosition(0.0f, 0.0f, 4.0f);
			camera->AddComponent(new Gadget::CameraComponent(camera));

			Gadget::Scene* testScene = new Gadget::Scene(SID("TestScene"));
			testScene->CreateObject(cube);
			testScene->CreateObject(light);
			testScene->CreateObject(camera);

			Gadget::App::GetInstance()->GetSceneManager()->AddScene(testScene);
		}
	};
}

#endif //!EXAMPLE_GAME_H
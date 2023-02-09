#ifndef PONG_SCENE_H
#define PONG_SCENE_H

#include <Game/Scene.h>
#include <Graphics/Components/CameraComponent.h>
#include <Graphics/Components/LightComponent.h>
#include <Graphics/Components/RenderComponent.h>
#include <Graphics/Components/SkyboxComponent.h>
#include <Physics/Rigidbody.h>

#include "PaddleObject.h"

namespace Pong{
	class PongScene : public Gadget::Scene{
	public:
		PongScene() : Gadget::Scene(SID("PongScene")){}

	protected:
		virtual void SetToDefaultState() override{
			Gadget::Scene::SetToDefaultState();

			Gadget::GameObject* camera = new Gadget::GameObject();
			camera->SetPosition(0.0f, 0.0f, 0.0f);
			camera->AddComponent(new Gadget::CameraComponent(camera, Gadget::Camera::Projection::Orthographic));
			CreateObject(camera);

			Gadget::GameObject* leftPaddle = new PaddleObject(SID("LeftPaddle"), 1);
			leftPaddle->SetPosition(-5.0f, 0.0f, 0.0f);
			CreateObject(leftPaddle);

			Gadget::GameObject* rightPaddle = new PaddleObject(SID("RightPaddle"), 2);
			rightPaddle->SetPosition(15.0f, 0.0f, 0.0f);
			CreateObject(rightPaddle);
		}
	};
}

#endif //!PONG_SCENE_H
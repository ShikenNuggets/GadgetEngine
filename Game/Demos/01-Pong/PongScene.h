#ifndef PONG_SCENE_H
#define PONG_SCENE_H

#include <Game/Scene.h>
#include <Graphics/Components/CameraComponent.h>
#include <Graphics/Components/RenderComponent.h>
#include <Physics/BoxCollider2D.h>
#include <Physics/Rigidbody.h>

#include "BallController.h"
#include "PaddleObject.h"

namespace Pong{
	class PongScene : public Gadget::Scene{
	public:
		PongScene() : Gadget::Scene(SID("PongScene")){}

	protected:
		virtual void SetToDefaultState() override{
			Gadget::Scene::SetToDefaultState();

			auto camera = new Gadget::GameObject();
			camera->SetPosition(0.0f, 0.0f, 0.0f);
			camera->AddComponent(new Gadget::CameraComponent(camera, Gadget::Camera::Projection::Orthographic));
			CreateObject(camera);

			auto leftPaddle = new PaddleObject(SID("LeftPaddle"), 1);
			leftPaddle->SetPosition(-7.5f, 0.0f, 0.0f);
			CreateObject(leftPaddle);

			auto rightPaddle = new PaddleObject(SID("RightPaddle"), 2);
			rightPaddle->SetPosition(7.5f, 0.0f, 0.0f);
			CreateObject(rightPaddle);

			auto ball = new Gadget::GameObject();
			ball->SetScale(0.5f);
			ball->AddComponent(new Gadget::RenderComponent(ball, SID("CubeModel"), Gadget::Color::White(), SID("ColorShader")));
			ball->AddComponent(new Gadget::Rigidbody(ball, 1.0f, false));
			ball->AddComponent(new Gadget::BoxCollider2D(ball, 1.0f, 1.0f, true));
			ball->AddComponent(new BallController(ball, 200.0f));
			CreateObject(ball);
		}
	};
}

#endif //!PONG_SCENE_H
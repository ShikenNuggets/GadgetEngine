#ifndef PONG_SCENE_H
#define PONG_SCENE_H

#include <Game/Scene.h>
#include <Graphics/Components/CameraComponent.h>
#include <Graphics/Components/RenderComponent.h>
#include <Graphics/GUI/CanvasSceneComponent.h>
#include <Graphics/GUI/FpsDisplayElement.h>
#include <Graphics/GUI/GuiTextElement.h>
#include <Physics/BoxCollider2D.h>
#include <Physics/Rigidbody.h>

#include "BallController.h"
#include "PaddleObject.h"
#include "PongSceneHandler.h"

namespace Pong{
	class PongScene : public Gadget::Scene{
	public:
		PongScene() : Gadget::Scene(SID("PongScene")){}

	protected:
		virtual void SetToDefaultState() final override{
			constexpr float playAreaWidth = 21.0f;
			constexpr float playAreaHeight = 12.0f;

			Gadget::Scene::SetToDefaultState();

			AddSceneComponent(new PongSceneHandler(this));

			Gadget::GuiCanvas* gc = new Gadget::GuiCanvas(SID("MainCanvas"));
			gc->AddElement(new Gadget::GuiTextElement(SID("Score1"), "0", SID("ArialFont"), Gadget::Vector2(-1.5f, 0.85f), Gadget::Vector2(0.125f, 0.125f), Gadget::GuiAnchor::Center));
			gc->AddElement(new Gadget::GuiTextElement(SID("Score2"), "0", SID("ArialFont"), Gadget::Vector2(1.5f, 0.85f), Gadget::Vector2(0.125f, 0.125f), Gadget::GuiAnchor::Center));
			gc->AddElement(new Gadget::FpsDisplayElement(SID("FPS"), SID("ArialFont"), Gadget::Vector2(1.65f, -0.95f), Gadget::Vector2(0.125f, 0.125f), Gadget::GuiAnchor::Center));
			AddSceneComponent(new Gadget::CanvasSceneComponent(this, gc));

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
			ball->AddTag(SID("Ball"));
			ball->SetScale(0.5f);
			ball->AddComponent(new Gadget::RenderComponent(ball, SID("CubeModel"), Gadget::Color::White(), SID("ColorShader")));
			ball->AddComponent(new Gadget::Rigidbody(ball, 1.0f, false, Gadget::FreezeRotationType::FreezeAll));
			ball->AddComponent(new Gadget::BoxCollider2D(ball, 1.0f, 1.0f, true));
			ball->AddComponent(new BallController(ball, 2.0f, playAreaWidth, playAreaHeight));
			CreateObject(ball);

			auto topWall = new Gadget::GameObject(SID("TopWall"));
			topWall->AddTag(SID("Wall"));
			topWall->SetPosition(Gadget::Vector3(0.0f, playAreaHeight / 2.0f, 0.0f));
			topWall->SetScale(Gadget::Vector3(30.0f, 0.25f, 1.0f));
			topWall->AddComponent(new Gadget::RenderComponent(topWall, SID("CubeModel"), Gadget::Color::DarkGray(), SID("ColorShader")));
			topWall->AddComponent(new Gadget::BoxCollider2D(topWall));
			CreateObject(topWall);

			auto bottomWall = new Gadget::GameObject(SID("BottomWall"));
			bottomWall->AddTag(SID("Wall"));
			bottomWall->SetPosition(Gadget::Vector3(0.0f, -playAreaHeight / 2.0f, 0.0f));
			bottomWall->SetScale(Gadget::Vector3(30.0f, 0.25f, 1.0f));
			bottomWall->AddComponent(new Gadget::RenderComponent(bottomWall, SID("CubeModel"), Gadget::Color::DarkGray(), SID("ColorShader")));
			bottomWall->AddComponent(new Gadget::BoxCollider2D(bottomWall));
			CreateObject(bottomWall);

			auto leftGoal = new Gadget::GameObject();
			leftGoal->AddTag(SID("LeftGoal"));
			leftGoal->SetPosition(Gadget::Vector3(-playAreaWidth / 2.0f, 0.0f, 0.0f));
			leftGoal->SetScale(Gadget::Vector3(0.25f, 15.0f, 1.0f));
			leftGoal->AddComponent(new Gadget::RenderComponent(leftGoal, SID("CubeModel"), Gadget::Color::DarkGray(), SID("ColorShader")));
			leftGoal->AddComponent(new Gadget::BoxCollider2D(leftGoal));
			CreateObject(leftGoal);

			auto rightGoal = new Gadget::GameObject();
			rightGoal->AddTag(SID("RightGoal"));
			rightGoal->SetPosition(Gadget::Vector3(playAreaWidth / 2.0f, 0.0f, 0.0f));
			rightGoal->SetScale(Gadget::Vector3(0.25f, 15.0f, 1.0f));
			rightGoal->AddComponent(new Gadget::RenderComponent(rightGoal, SID("CubeModel"), Gadget::Color::DarkGray(), SID("ColorShader")));
			rightGoal->AddComponent(new Gadget::BoxCollider2D(rightGoal));
			CreateObject(rightGoal);
		}
	};
}

#endif //!PONG_SCENE_H
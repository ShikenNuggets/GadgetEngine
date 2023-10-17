#ifndef PONG_PADDLE_OBJECT_H
#define PONG_PADDLE_OBJECT_H

#include <Game/GameObject.h>
#include <Graphics/Components/RenderComponent.h>
#include <Physics/BoxCollider2D.h>

#include "PaddleAI.h"
#include "PaddleController.h"
#include "PongState.h"

namespace Pong{
	class PaddleObject : public Gadget::GameObject{
	public:
		PaddleObject(Gadget::StringID name_ = SID("Paddle"), unsigned int player_ = 1) : GameObject(name_){
			AddTag(SID("Paddle"));
			SetScale(Gadget::Vector3(0.5f, 3.0f, 1.0f));
			AddComponent(new Gadget::RenderComponent(this, SID("CubeModel"), Gadget::Color::White(), SID("ColorShader")));
			AddComponent(new Gadget::Rigidbody(this, 1.0f, false, Gadget::FreezeRotationType::FreezeAll));
			AddComponent(new Gadget::BoxCollider2D(this));

			if(player_ == 2 && (PongState::currentMode == GameMode::SoloEasy || PongState::currentMode == GameMode::SoloHard)){
				AddComponent(new PaddleAI(this, player_));
			}else{
				AddComponent(new PaddleController(this, player_));
			}
		}
	};
}

#endif //!PONG_PADDLE_OBJECT_H
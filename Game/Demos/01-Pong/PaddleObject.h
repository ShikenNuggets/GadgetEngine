#ifndef PONG_PADDLE_OBJECT_H
#define PONG_PADDLE_OBJECT_H

#include <Game/GameObject.h>
#include <Graphics/Components/RenderComponent.h>

#include "PaddleController.h"

namespace Pong{
	class PaddleObject : public Gadget::GameObject{
	public:
		PaddleObject(Gadget::StringID name_ = SID("Paddle"), unsigned int player_ = 1) : GameObject(name_){
			SetScale(Gadget::Vector3(0.5f, 3.0f, 1.0f));
			AddComponent(new Gadget::RenderComponent(this, SID("CubeModel"), Gadget::Color::White(), SID("ColorShader")));
			AddComponent(new PaddleController(this, player_));
		}
	};
}

#endif //!PONG_PADDLE_OBJECT_H
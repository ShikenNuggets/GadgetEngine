#ifndef PONG_PADDLE_CONTROLLER_H
#define PONG_PADDLE_CONTROLLER_H

#include <App.h>
#include <Game/GameLogicComponent.h>

namespace Pong{
	class PaddleController : public Gadget::GameLogicComponent{
	public:
		PaddleController(Gadget::GameObject* parent_, unsigned int player_) : GameLogicComponent(parent_), paddleMoveSpeed(5.0f), player(player_){
			GADGET_BASIC_ASSERT(player == 1 || player == 2);
		}

		virtual void OnStart() override{
			startPosition = parent->GetPosition();
			GameLogicComponent::OnStart();
		}

		virtual void OnUpdate(float deltaTime_) override{
			float moveAxis = 0.0f;
			if(player == 1){
				moveAxis = Gadget::App::GetInput().GetAxis(SID("LeftPaddleMove"));
			}else{
				moveAxis = Gadget::App::GetInput().GetAxis(SID("RightPaddleMove"));
			}

			parent->Translate(Gadget::Vector3(0.0f, moveAxis * paddleMoveSpeed * deltaTime_, 0.0f));

			GameLogicComponent::OnUpdate(deltaTime_);
		}

		void Reset(){
			parent->SetPosition(startPosition);
		}

	private:
		const float paddleMoveSpeed;
		const unsigned int player;
		Gadget::Vector3 startPosition;
	};
}

#endif //!PONG_PADDLE_CONTROLLER_H
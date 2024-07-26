#ifndef STAR_HAWX_PLAYER_CAMERA_H
#define STAR_HAWX_PLAYER_CAMERA_H

namespace StarHawx{
	class PlayerCamera : public Gadget::GameLogicComponent{
	public:
		PlayerCamera(Gadget::GameObject* parent_) : GameLogicComponent(SID("PlayerCamera"), parent_){}

		virtual void OnStart() final override{
			GameLogicComponent::OnStart();
		}

		virtual void OnUpdate(float deltaTime_) final override{}
	};
}

#endif //!STAR_HAWX_PLAYER_CAMERA_H
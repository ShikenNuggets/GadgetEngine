#include <Gadget.h>

namespace Example{
	static constexpr Gadget::StaticArray<Gadget::StringID, 3> gPlayerAnimClipNames{
		"IdleAnim"_sid,
		"StandUpAnim"_sid,
		"CrouchWalkAnim"_sid,
	};

	class PlayerController : public Gadget::GameLogicComponent{
	public:
		PlayerController(Gadget::GameObject* parent_) : GameLogicComponent(SID("CameraController"), parent_), animRender(nullptr), currentClipIndex(0){}
		virtual ~PlayerController() override = default;

		virtual void OnStart() override{
			animRender = parent->GetComponent<Gadget::AnimRenderComponent>();
			GADGET_BASIC_ASSERT(animRender != nullptr);

			GameLogicComponent::OnStart();
		}

		virtual void OnUpdate(float deltaTime_) override{
			GameLogicComponent::OnUpdate(deltaTime_);

			GADGET_BASIC_ASSERT(animRender != nullptr);
			if(animRender == nullptr){
				return;
			}

			if(Gadget::App::GetInput().GetButtonDown(Gadget::ButtonID::Keyboard_1) && currentClipIndex != 0){
				currentClipIndex = 0;
				animRender->PlayClip(gPlayerAnimClipNames[currentClipIndex]);
			}else if(Gadget::App::GetInput().GetButtonDown(Gadget::ButtonID::Keyboard_2) && currentClipIndex != 1){
				currentClipIndex = 1;
				animRender->PlayClip(gPlayerAnimClipNames[currentClipIndex]);
			}else if(Gadget::App::GetInput().GetButtonDown(Gadget::ButtonID::Keyboard_3) && currentClipIndex != 2){
				currentClipIndex = 2;
				animRender->PlayClip(gPlayerAnimClipNames[currentClipIndex]);
			}else if(Gadget::App::GetInput().GetButtonDown(Gadget::ButtonID::Keyboard_0)){
				currentClipIndex = -1;
				animRender->Stop();
			}
		}

	private:
		Gadget::AnimRenderComponent* animRender;
		int currentClipIndex;
	};
}
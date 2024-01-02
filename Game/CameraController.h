#ifndef EXAMPLE_CAMERA_CONTROLLER_H
#define EXAMPLE_CAMERA_CONTROLLER_H

#include <App.h>
#include <Core/Time.h>
#include <Game/GameLogicComponent.h>
#include <Game/GameObject.h>
#include <Input/Input.h>

namespace Example{
	class CameraController : public Gadget::GameLogicComponent{
	public:
		CameraController(Gadget::GameObject* parent_) : GameLogicComponent(SID("CameraController"), parent_), input(Gadget::App::GetInput()){}
		
		CameraController(const Gadget::ComponentProperties& props_) : GameLogicComponent(props_), input(Gadget::App::GetInput()){
			Deserialize(props_);
		}
		
		virtual ~CameraController() override{}

		virtual void OnUpdate(float deltaTime_) override{
			if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_W)){
				parent->Translate(Gadget::Vector3(0.0f, 0.0f, -2.5f * deltaTime_));
			}else if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_S)){
				parent->Translate(Gadget::Vector3(0.0f, 0.0f, 2.5f * deltaTime_));
			}

			if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_A)){
				parent->Translate(Gadget::Vector3(-2.5f * deltaTime_, 0.0f, 0.0f));
			}else if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_D)){
				parent->Translate(Gadget::Vector3(2.5f * deltaTime_, 0.0f));
			}

			if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Up)){
				parent->Rotate(Gadget::Euler(30 * deltaTime_, 0.0f, 0.0f));
			}else if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Down)){
				parent->Rotate(Gadget::Euler(-30 * deltaTime_, 0.0f, 0.0f));
			}

			if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Left)){
				parent->Rotate(Gadget::Euler(0.0f, 30 * deltaTime_, 0.0f));
			}else if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Right)){
				parent->Rotate(Gadget::Euler(0.0f, -30 * deltaTime_, 0.0f));
			}

			if(input.GetButtonDown(Gadget::ButtonID::Keyboard_R)){
				parent->SetPosition(Gadget::Vector3(0.0f, 0.0f, 4.0f));
				parent->SetRotation(Gadget::Quaternion::Identity());

				Gadget::App::GetSceneManager().RequestSceneLoad(0);
			}
		}

		virtual Gadget::ComponentProperties Serialize() const override{ return GameLogicComponent::Serialize(); }

	protected:
		virtual void Deserialize(const Gadget::ComponentProperties& props_) override{}

	private:
		Gadget::Input& input;
	};
}

#endif //!EXAMPLE_CAMERA_CONTROLLER_H
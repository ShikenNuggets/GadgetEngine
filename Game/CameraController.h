#ifndef EXAMPLE_CAMERA_CONTROLLER_H
#define EXAMPLE_CAMERA_CONTROLLER_H

#include <Core/Time.h>
#include <Game/GameLogicComponent.h>
#include <Game/GameObject.h>
#include <Input/Input.h>

namespace Example{
	class CameraController : public Gadget::GameLogicComponent{
	public:
		CameraController(Gadget::GameObject* parent_) : GameLogicComponent(parent_){}
		virtual ~CameraController() override{}

		virtual void OnUpdate(float deltaTime_) override{
			if(Gadget::Input::GetInstance()->GetButtonHeld(Gadget::ButtonID::Keyboard_W)){
				parent->Translate(Gadget::Vector3(0.0f, 0.0f, -2.5f * deltaTime_));
			}else if(Gadget::Input::GetInstance()->GetButtonHeld(Gadget::ButtonID::Keyboard_S)){
				parent->Translate(Gadget::Vector3(0.0f, 0.0f, 2.5f * deltaTime_));
			}

			if(Gadget::Input::GetInstance()->GetButtonHeld(Gadget::ButtonID::Keyboard_A)){
				parent->Translate(Gadget::Vector3(-2.5f * deltaTime_, 0.0f, 0.0f));
			}else if(Gadget::Input::GetInstance()->GetButtonHeld(Gadget::ButtonID::Keyboard_D)){
				parent->Translate(Gadget::Vector3(2.5f * deltaTime_, 0.0f));
			}

			if(Gadget::Input::GetInstance()->GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Up)){
				parent->Rotate(Gadget::Euler(30 * deltaTime_, 0.0f, 0.0f));
			}else if(Gadget::Input::GetInstance()->GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Down)){
				parent->Rotate(Gadget::Euler(-30 * deltaTime_, 0.0f, 0.0f));
			}

			if(Gadget::Input::GetInstance()->GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Left)){
				parent->Rotate(Gadget::Euler(0.0f, 30 * deltaTime_, 0.0f));
			}else if(Gadget::Input::GetInstance()->GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Right)){
				parent->Rotate(Gadget::Euler(0.0f, -30 * deltaTime_, 0.0f));
			}

			if(Gadget::Input::GetInstance()->GetButtonDown(Gadget::ButtonID::Keyboard_R)){
				parent->SetPosition(Gadget::Vector3(0.0f, 0.0f, 4.0f));
				parent->SetRotation(Gadget::Quaternion::Identity());
			}
		}
	};
}

#endif //!EXAMPLE_CAMERA_CONTROLLER_H
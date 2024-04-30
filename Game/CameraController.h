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
			constexpr float moveSpeed = 2.5f;
			constexpr float rotateSpeed = 45.0f;

			if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_W)){
				parent->Translate(parent->GetTransform().Forward() * moveSpeed * deltaTime_);
			}else if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_S)){
				parent->Translate(parent->GetTransform().Forward() * -moveSpeed * deltaTime_);
			}

			if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_A)){
				parent->Translate(parent->GetTransform().Right() * -moveSpeed * deltaTime_);
			}else if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_D)){
				parent->Translate(parent->GetTransform().Right() * moveSpeed * deltaTime_);
			}

			if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_Q)){
				parent->Translate(Gadget::Vector3::Up() * -moveSpeed * deltaTime_);
			}else if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_E)){
				parent->Translate(Gadget::Vector3::Up() * moveSpeed * deltaTime_);
			}

			const Gadget::Euler oldRotation = parent->GetRotation().ToEuler();

			if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Up)){
				//This is a little unorthodox but working with Euler angles directly ensures that the camera never rolls
				parent->SetRotation(Gadget::Euler(oldRotation.x + rotateSpeed * deltaTime_, oldRotation.y, 0.0f));
			}else if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Down)){
				parent->SetRotation(Gadget::Euler(oldRotation.x - rotateSpeed * deltaTime_, oldRotation.y, 0.0f));
			}

			if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Left)){
				parent->SetRotation(Gadget::Euler(oldRotation.x, oldRotation.y + rotateSpeed * deltaTime_, 0.0f));
			}else if(input.GetButtonHeld(Gadget::ButtonID::Keyboard_Arrow_Right)){
				parent->SetRotation(Gadget::Euler(oldRotation.x, oldRotation.y - rotateSpeed * deltaTime_, 0.0f));
			}

			if(input.GetButtonDown(Gadget::ButtonID::Keyboard_R)){
				parent->SetPosition(Gadget::Vector3(0.0f, 0.0f, 4.0f));
				parent->SetRotation(Gadget::Quaternion::Identity());

				Gadget::App::GetSceneManager().RequestSceneLoad(0);
			}
		}

		virtual Gadget::ComponentProperties Serialize() const override{ return GameLogicComponent::Serialize(); }

	protected:
		virtual void Deserialize([[maybe_unused]] const Gadget::ComponentProperties& props_) override{}

	private:
		Gadget::Input& input;
	};
}

#endif //!EXAMPLE_CAMERA_CONTROLLER_H
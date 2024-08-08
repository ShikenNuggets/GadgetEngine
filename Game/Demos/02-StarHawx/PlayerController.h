#ifndef STAR_HAWX_PLAYER_CONTROLLER_H
#define STAR_HAWX_PLAYER_CONTROLLER_H

#include <Game/GameLogicComponent.h>

#include "ProjectileObject.h"
#include "StarHawxDefines.h"

namespace StarHawx{
	class PlayerController : public Gadget::GameLogicComponent{
	public:
		PlayerController(Gadget::GameObject* parent_) : GameLogicComponent(SID("PlayerController"), parent_), rigidbody(nullptr), flightSpeed(15.0f), tiltDir(TiltDir::None), tiltAngle(35.0f), tiltLerpSpeed(0.5f), tiltLerpTimer(0.0f), isLerping(false){}

		virtual void OnStart() final override{
			GADGET_BASIC_ASSERT(parent != nullptr);
			rigidbody = parent->GetComponent<Gadget::Rigidbody>();
			GADGET_BASIC_ASSERT(rigidbody != nullptr);

			GameLogicComponent::OnStart();
		}

		virtual void OnUpdate(float deltaTime_) final override{
			GADGET_BASIC_ASSERT(rigidbody != nullptr);

			float upDownMove = Gadget::App::GetInput().GetAxis(StarHawx::UpDownMoveAxis);
			float leftRightMove = Gadget::App::GetInput().GetAxis(StarHawx::LeftRightMoveAxis);

			Gadget::Vector3 velocity = Gadget::Vector3(leftRightMove, upDownMove, 0.0f);
			if(!Gadget::Math::IsNearZero(upDownMove) || !Gadget::Math::IsNearZero(leftRightMove)){
				velocity.Normalize();
			}
			velocity *= flightSpeed;

			rigidbody->ClearForces();
			rigidbody->SetVelocity(velocity);

			if(upDownMove > 0.0f && Gadget::Math::IsNearZero(leftRightMove)){
				SetNewTiltDir(TiltDir::Up);
			}else if(upDownMove > 0.0f && leftRightMove > 0.0f){
				SetNewTiltDir(TiltDir::UpRight);
			}else if(Gadget::Math::IsNearZero(upDownMove) && leftRightMove > 0.0f){
				SetNewTiltDir(TiltDir::Right);
			}else if(upDownMove < 0.0f && leftRightMove > 0.0f){
				SetNewTiltDir(TiltDir::DownRight);
			}else if(upDownMove < 0.0f && Gadget::Math::IsNearZero(leftRightMove)){
				SetNewTiltDir(TiltDir::Down);
			}else if(upDownMove < 0.0f && leftRightMove < 0.0f){
				SetNewTiltDir(TiltDir::DownLeft);
			}else if(Gadget::Math::IsNearZero(upDownMove) && leftRightMove < 0.0f){
				SetNewTiltDir(TiltDir::Left);
			}else if(upDownMove > 0.0f && leftRightMove < 0.0f){
				SetNewTiltDir(TiltDir::UpLeft);
			}else{
				SetNewTiltDir(TiltDir::None);
			}

			if(isLerping){
				tiltLerpTimer -= deltaTime_;
				
				if(tiltLerpTimer <= 0.0f){
					parent->SetRotation(targetTilt);
					isLerping = false;
				}else{
					parent->SetRotation(Gadget::Euler::Lerp(startTilt, targetTilt, 1.0f - (tiltLerpTimer / tiltLerpSpeed)));
				}
			}

			if(Gadget::App::GetInput().GetButtonDown(StarHawx::ShootButton)){
				SpawnProjectile();
			}

			GameLogicComponent::OnUpdate(deltaTime_);
		}

	protected:
		Gadget::Rigidbody* rigidbody;

		float flightSpeed;
		TiltDir tiltDir;
		float tiltAngle;

		Gadget::Euler startTilt;
		Gadget::Euler targetTilt;
		float tiltLerpSpeed;
		float tiltLerpTimer;
		bool isLerping;

		void SetNewTiltDir(TiltDir newDir_){
			if(newDir_ == tiltDir){
				return;
			}

			tiltDir = newDir_;
			isLerping = true;
			tiltLerpTimer = tiltLerpSpeed;
			startTilt = parent->GetRotation().ToEuler();

			switch(tiltDir){
				case TiltDir::Up:
					targetTilt = Gadget::Euler(-tiltAngle, startTilt.y, 0.0f);
					break;
				case TiltDir::UpRight:
					targetTilt = Gadget::Euler(-tiltAngle, startTilt.y, tiltAngle / 1.5f);
					break;
				case TiltDir::Right:
					targetTilt = Gadget::Euler(0.0f, startTilt.y, tiltAngle / 1.5f);
					break;
				case TiltDir::DownRight:
					targetTilt = Gadget::Euler(tiltAngle, startTilt.y, tiltAngle / 1.5f);
					break;
				case TiltDir::Down:
					targetTilt = Gadget::Euler(tiltAngle, startTilt.y, 0.0f);
					break;
				case TiltDir::DownLeft:
					targetTilt = Gadget::Euler(tiltAngle, startTilt.y, -tiltAngle / 1.5f);
					break;
				case TiltDir::Left:
					targetTilt = Gadget::Euler(0.0f, startTilt.y, -tiltAngle / 1.5f);
					break;
				case TiltDir::UpLeft:
					targetTilt = Gadget::Euler(-tiltAngle, startTilt.y, -tiltAngle / 1.5f);
					break;
				default:
					targetTilt = Gadget::Euler(0.0f, startTilt.y, 0.0f);
					break;
			}
		}

		void SpawnProjectile(){
			Gadget::Scene* currentScene = Gadget::App::GetSceneManager().CurrentScene();
			GADGET_BASIC_ASSERT(currentScene != nullptr);
			if(currentScene == nullptr){
				return;
			}

			currentScene->CreateObject(new ProjectileObject(parent->GetPosition() + Gadget::Vector3(0.0f, 0.0f, -5.0f), parent->GetRotation()));
		}
	};
}

#endif //!STAR_HAWX_PLAYER_CONTROLLER_H
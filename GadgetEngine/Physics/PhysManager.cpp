#include "PhysManager.h"

#include "App.h"
#include "Config.h"
#include "Collider.h"
#include "CollisionSystem.h"
#include "Rigidbody.h"
#include "Game/GameLogicComponent.h"

using namespace Gadget;

PhysManager::PhysManager(){}

PhysManager::~PhysManager(){}

void PhysManager::Update(Scene* scene_, float deltaTime_){
	const float gravityConstant = static_cast<float>(App::GetConfig().GetOptionFloat(EngineVars::Physics::gravityConstantKey));

	const auto rbs = scene_->GetAllComponentsInScene<Rigidbody>(); //TODO - This is slow
	for(const auto& rb : rbs){
		if(rb->UseGravity()){
			rb->AddForce(Vector3(0.0f, gravityConstant, 0.0f) * rb->GetMass()); //Multiply by mass to cancel out the division later (gravity ignores mass)
		}

		rb->Update(deltaTime_);
	}

	const auto cls = scene_->GetAllComponentsInScene<BoxCollider2D>(); //TODO - This is slow
	//TODO - This is *really* slow. It checks every collider in the scene against every other collider
	//This is bad enough in a small scene but it'll get exponentially worse as more colliders are added
	for(int i = 0; i < cls.size(); i++){
		for(int j = i + 1; j < cls.size(); j++){
			if(CollisionSystem::TestCollision(*cls[i], *cls[j])){
				HandleCollisionResponse(cls[i], cls[j]);
				HandleCollisionResponse(cls[j], cls[i]);
			}
		}
	}
}

void PhysManager::HandleCollisionResponse(Collider* collider_, Collider* other_){
	auto rb1 = collider_->GetParent()->GetComponent<Rigidbody>();
	auto lgs = collider_->GetParent()->GetComponents<GameLogicComponent>();

	Collision collision = Collision(other_->GetParent()->GetTags(), collider_->IsTrigger() || other_->IsTrigger());

	if(!collision.isTrigger){
		if(rb1 != nullptr){
			//TODO - Physics collision response
		}
	}

	for(const auto& lg : lgs){
		lg->AddCollisionToHandle(collision);
	}
}
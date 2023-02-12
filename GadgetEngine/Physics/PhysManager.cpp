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
	auto rb = collider_->GetParent()->GetComponent<Rigidbody>();
	auto lgs = collider_->GetParent()->GetComponents<GameLogicComponent>();

	Collision collision;
	collision.otherTags = other_->GetParent()->GetTags();
	collision.otherPos = other_->GetParent()->GetPosition();
	collision.otherScale = other_->GetColliderSize();
	collision.collisionVector = other_->GetParent()->GetPosition() - collider_->GetParent()->GetPosition();
	collision.overlapAmount = CollisionSystem::CalculateOverlapAmount(collision.collisionVector.Normalized(), *dynamic_cast<BoxCollider2D*>(collider_), *dynamic_cast<BoxCollider2D*>(other_));
	collision.isTrigger = collider_->IsTrigger() || other_->IsTrigger();

	if(!collision.isTrigger){
		if(rb != nullptr){
			//TODO - Physics collision response
		}
	}

	for(const auto& lg : lgs){
		lg->AddCollisionToHandle(collision);
	}
}
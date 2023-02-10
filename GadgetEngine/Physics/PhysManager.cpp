#include "PhysManager.h"

#include "App.h"
#include "Config.h"
#include "Collider.h"
#include "CollisionSystem.h"
#include "Rigidbody.h"

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
	for(int i = 0; i < cls.size(); i++){
		for(int j = i + 1; j < cls.size(); j++){
			if(CollisionSystem::TestCollision(*cls[i], *cls[j])){
				auto rb1 = cls[i]->GetParent()->GetComponent<Rigidbody>();
				if(rb1 != nullptr){
					//TODO - Handle collision response
				}

				auto rb2 = cls[j]->GetParent()->GetComponent<Rigidbody>();
				if(rb2 != nullptr){
					//TODO - Handle collision response
				}
			}
		}
	}
}
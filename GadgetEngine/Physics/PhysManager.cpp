#include "PhysManager.h"

#include "App.h"
#include "Config.h"
#include "Physics/Rigidbody.h"

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
}